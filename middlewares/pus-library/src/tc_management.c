/**
 * @file    tc_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC management
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tc_management.h"
#include "tools/tables_management.h"
#include "tools/schedule_management.h"
#include "tools/crc_computation.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t FormatTC(pusTC_t *tc);
static returnCode_t CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error);
static void EraseTC(pusTC_t *tc);
static returnCode_t SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, deviceNo_t dev_ack);
static returnCode_t SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, deviceNo_t dev_ack, pusAcceptanceError_t acceptance_error);
static returnCode_t SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack);
static returnCode_t SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack, pusExecutionError_t execution_error);
static returnCode_t CheckCRC(pusTC_t *tc);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitTCReceiveContext(pusReceiveContext_t *receive_context)
 * @brief       Function that initialise the receive context for TC handling
 * @param[in]   receive_context Execution context for the task dealing with TC execution
 * @retval      #RET_INVALID_PARAM if a pointer is a null pointer or routing table size is null
 * @retval      #RET_ERROR if initialisation failed because of device binding or execution table initialisation
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitTCReceiveContext(pusReceiveContext_t *receive_context)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t device_status;

    // Function Core
    if ((receive_context != NULL) && (receive_context->routing_table != NULL) && (receive_context->routing_table_size != 0u) && (receive_context->tc != NULL))
    {
        // First initialise the routing table
        return_value = InitRoutingTable(receive_context->routing_table, receive_context->routing_table_size);
        if (return_value == RET_SUCCESSFUL)
        {
            // If nothing wrong happened, initialise the RX resource (buffer or peripheral)
            device_status = DeviceOpen(&receive_context->dev_rx, receive_context->rx_type, receive_context->ref_rx);

            // If nothing wrong happened and a ACK TM buffer is required, initialise device for ACK TM buffer
            if ((device_status == RET_SUCCESSFUL) && (receive_context->buffer_ack != NO_BUFFER))
            {
                device_status = DeviceOpen(&receive_context->dev_ack, DEVICE_TYPE_BUFFER, receive_context->buffer_ack);
            }

            // Finally check everything went right
            if (device_status == RET_SUCCESSFUL)
            {
                receive_context->status = PUS_CONTEXT_INITIALIZED;
            }
            else
            {
                return_value = RET_ERROR;
                receive_context->status = PUS_CONTEXT_ERROR;
            }
        }
        else
        {
            receive_context->status = PUS_CONTEXT_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn          ReceiveTC(pusReceiveContext_t *receive_context)
 * @brief       Function that get a TC and and routes it toward it's corresponding task
 * @param[in]   receive_context Execution context for the task dealing with TC execution
 * @retval      #RET_INVALID_PARAM if receive_context is not initialised
 * @retval      #RET_NOT_AVAILABLE if there is no TC available
 * @retval      #RET_ERROR if receiving the TC is not working
 * @retval      #RET_ERROR if cannot format TC
 * @retval      #RET_ERROR if cannot write TC into it's device
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReceiveTC(pusReceiveContext_t *receive_context)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTM_t acceptance_tm = {0};
    pusAcceptanceError_t acceptance_error = PUS_ACCEPTANCE_NO_ERROR;
    pusTC_t *tc = receive_context->tc; // Renaming for easier usage

    // Function Core
    if (receive_context->status == PUS_CONTEXT_INITIALIZED)
    {
        // First, we check if there is a TC.
        return_value = DeviceRead(receive_context->dev_rx, (data_t)tc, TC_MAX_SIZE);
        if (return_value == RET_SUCCESSFUL)
        {
            // First, we check the validity of the TC.
            return_value = CheckTCValidity(tc, &acceptance_error);
            if (return_value == RET_SUCCESSFUL)
            {
                // If TC is valid, we format the TC because of endianness.
                return_value = FormatTC(tc);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Then, we route the TC toward the task that will execute it.
                    deviceNo_t dev_route = 0u;
                    uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc->spp_header.packet_id), tc->tc_header.service, tc->tc_header.subservice);
                    return_value = RouteSearch((pusRoutingTable_t *)receive_context->routing_table, receive_context->routing_table_size, key, &dev_route);
                    if (return_value == RET_SUCCESSFUL)
                    {
                        // Acknowledge TC
                        (void)SendAcptAckTM(tc, &acceptance_tm, receive_context->dev_ack);

                        // Send TC to the task that will execute it
                        returnCode_t test_write = DeviceWrite(dev_route, (data_t)tc, TC_MAX_SIZE);
                        if (test_write != RET_SUCCESSFUL)
                        {
                            return_value = RET_ERROR;
                        }
                    }
                    else
                    {
                        // Bad routing so TC non acknowleded
                        (void)SendAcptNackTM(tc, &acceptance_tm, receive_context->dev_ack, PUS_ACCEPTANCE_INVALID_ROUTE);
                    }
                }
                else
                {
                    // Can't format so TC non acknowleded
                    (void)SendAcptNackTM(tc, &acceptance_tm, receive_context->dev_ack, PUS_ACCEPTANCE_CANT_FORMAT);
                }
            }
            else
            {
                // Invalid TC, TC will be non-acknowledged.
                (void)SendAcptNackTM(tc, &acceptance_tm, receive_context->dev_ack, acceptance_error);
            }

            // We erase TC for next call;
            EraseTC(tc);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InitTCExecutionContext(pusExecutionContext_t *execution_context)
 * @brief       Function that initialise the execution context for TC handling
 * @param[in]   execution_context Execution context for the task dealing with TC execution
 * @retval      #RET_INVALID_PARAM if a pointer is a null pointer or routing table size is null
 * @retval      #RET_ERROR if initialisation failed because of device binding or execution table initialisation
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitTCExecutionContext(pusExecutionContext_t *execution_context)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    returnCode_t device_status;

    // Function Core
    if ((execution_context != NULL) && (execution_context->execution_table != NULL) && (execution_context->execution_table_size != 0u))
    {
        // First initialise the execution table
        return_value = InitExecutionTable(execution_context->execution_table, execution_context->execution_table_size);
        if (return_value == RET_SUCCESSFUL)
        {
            // If nothing wrong happened and a TC buffer is requested, initialise device for TM buffer
            if (execution_context->buffer_tc != NO_BUFFER)
            {
                device_status = DeviceOpen(&execution_context->dev_tc, DEVICE_TYPE_BUFFER, execution_context->buffer_tc);
            }

            // If nothing wrong happened and a TM buffer is requested, initialise device for TM buffer
            if ((device_status == RET_SUCCESSFUL) && (execution_context->buffer_tm != NO_BUFFER))
            {
                device_status = DeviceOpen(&execution_context->dev_tm, DEVICE_TYPE_BUFFER, execution_context->buffer_tm);
            }

            // If nothing wrong happened and a ACK TM buffer is required, initialise device for ACK TM buffer
            if ((device_status == RET_SUCCESSFUL) && (execution_context->buffer_ack != NO_BUFFER))
            {
                device_status = DeviceOpen(&execution_context->dev_ack, DEVICE_TYPE_BUFFER, execution_context->buffer_ack);
            }

            // Finally check everything went right
            if (device_status == RET_SUCCESSFUL)
            {
                execution_context->status = PUS_CONTEXT_INITIALIZED;
            }
            else
            {
                return_value = RET_ERROR;
                execution_context->status = PUS_CONTEXT_ERROR;
            }
        }
        else
        {
            execution_context->status = PUS_CONTEXT_ERROR;
        }
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}

/**
 * @fn          ExecuteTC(pusExecutionContext_t *execution_context)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_context Execution context for the task dealing with TC execution
 * @retval      #RET_INVALID_PARAM if execution_context is empty or contains an empty field
 * @retval      #RET_ERROR if cannot recognize TC or has an error with device management
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ExecuteTC(pusExecutionContext_t *execution_context)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction = NULL; // cppcheck-suppress [misra-c2012-17.7,unmatchedSuppression]; False positive because ExecutionFunction is declared and not called

    // Function core
    if (execution_context->status == PUS_CONTEXT_INITIALIZED)
    {
        // First, we check if there is a TC.
        return_value = DeviceRead(execution_context->dev_tc, (data_t)&tc, TC_MAX_SIZE);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then, we find which TC we have to execute
            pusTMRequested_t tm_requested = 0u;
            uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
            return_value = ExecutionSearch(execution_context->execution_table, execution_context->execution_table_size, key, &tm_requested, &ExecutionFunction);
            if (return_value == RET_SUCCESSFUL)
            {
                // Now we execute the TC
                pusExecutionError_t error_code = PUS_EXECUTION_FAILED;
                return_value = ExecutionFunction(&tc, &tm, &error_code);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Acknowledge TC execution
                    (void)SendExecAckTM(&tc, &execution_tm, execution_context->dev_ack);

                    // Check if a specific TM has to be send
                    if (tm_requested == TM_REQUESTED)
                    {
                        // Send specific TM
                        returnCode_t test_write = DeviceWrite(execution_context->dev_tm, (data_t)&tm, TM_MAX_SIZE);
                        if (test_write != RET_SUCCESSFUL)
                        {
                            return_value = RET_ERROR;
                        }
                    }
                }
                else
                {
                    // TC Failed to be executed
                    (void)SendExecNackTM(&tc, &execution_tm, execution_context->dev_ack, error_code);
                }
            }
            else
            {
                // TC does not have execution procedure
                (void)SendExecNackTM(&tc, &execution_tm, execution_context->dev_ack, PUS_EXECUTION_UNAVAILABLE);
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              FormatTC(pusTC_t *tc)
 * @brief           Function that format TC the right way
 * @param[in,out]   tc Pointer to the TC we want to format
 * @retval          #RET_INVALID_PARAM if tc is null pointer
 * @retval          #RET_SUCCESSFUL else
 *
 * As we've done a silly memcpy with the uart driver, the
 * TC fields don't have the right endianness, or aren't in
 * the right place.
 */
static returnCode_t FormatTC(pusTC_t *tc)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (tc != NULL)
    {
        // Endianness Correction
        tc->spp_header.packet_id = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
        tc->spp_header.packet_sequence_control = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_sequence_control);
        tc->spp_header.packet_data_length = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length);
        tc->tc_header.source_id = HALF_WORD_BYTE_SWAP(tc->tc_header.source_id);

        // Put CRC at the right place
        tc->crc = (pusCRC_t)(tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] << 8u) +
                  (pusCRC_t)(tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 2u]);
        tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u] = 0u;
        tc->data[tc->spp_header.packet_data_length - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 2u] = 0u;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
 * @brief       Function that verifies if TC is valid (right version, type, size)
 * @param[in]   tc Pointer to the TC variable where we want to verify it validity.
 * @param[out]  error Pointer to pass error type to TM(1,2)
 * @retval      #RET_INVALID_PARAM if the TC is not well formated or CRC is invalid
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    uint16_t packet_id = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_id);
    uint16_t data_size = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length) + 1u;

    // Function Core
    // Check Packet Version Number
    if (((packet_id & PACKET_VERSION_NUMBER_MASK) >> PACKET_VERSION_NUMBER_OFFSET) == VALID_PACKET_VERSION_NUMBER)
    {
        // Check Packet Type
        if (((packet_id & PACKET_TYPE_MASK) >> PACKET_TYPE_OFFSET) == TC_TYPE)
        {
            // Check Secondary Header Presence
            if (((packet_id & HEADER_PRESENCE_MASK) >> HEADER_PRESENCE_OFFSET) == HEADER_PRESENT)
            {
                // Check Size
                if (data_size >= (TC_HEADER_SIZE + CRC_TRAILER_SIZE))
                {
                    // Check CRC
                    if (CheckCRC(tc) != RET_SUCCESSFUL)
                    {
                        return_value = RET_INVALID_PARAM;
                        *error = PUS_ACCEPTANCE_INVALID_CRC;
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                    *error = PUS_ACCEPTANCE_INVALID_FORMAT;
                }
            }
            else
            {
                return_value = RET_INVALID_PARAM;
                *error = PUS_ACCEPTANCE_INVALID_FORMAT;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
            *error = PUS_ACCEPTANCE_INVALID_FORMAT;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
        *error = PUS_ACCEPTANCE_INVALID_FORMAT;
    }

    return return_value;
}

/**
 * @fn              EraseTC(pusTC_t *tc)
 * @brief           Function that erase a TC, it fills it with zeros
 * @param[in,out]   tc Pointer to the TC we want to erase
 * @return          Nothing
 */
static void EraseTC(pusTC_t *tc)
{
    // Function Core
    (void)memset(tc, 0u, TC_MAX_SIZE);
}

/**
 * @fn          SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, deviceNo_t dev_ack)
 * @brief       This function send acceptance acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   dev_ack Device where the ACK TM will be sent
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if cannot write into device
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, deviceNo_t dev_ack)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        return_value = BuildS1SS1(tc, acceptance_tm);
        if (return_value == RET_SUCCESSFUL)
        {
            returnCode_t test_write = DeviceWrite(dev_ack, (data_t)acceptance_tm, TM_MAX_SIZE);
            if (test_write != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
 * @brief       This function send acceptance non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   dev_ack Device where the ACK TM will be sent
 * @param[in]   acceptance_error Code explaining why we nack the TC
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if cannot write into device
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, deviceNo_t dev_ack, pusAcceptanceError_t acceptance_error)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        return_value = BuildS1SS2(tc, acceptance_tm, acceptance_error);
        if (return_value == RET_SUCCESSFUL)
        {
            returnCode_t test_write = DeviceWrite(dev_ack, (data_t)acceptance_tm, TM_MAX_SIZE);
            if (test_write != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack)
 * @brief       This function send execution acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   dev_ack Device where the ACK TM will be sent
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if cannot write into device
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        return_value = BuildS1SS7(tc, execution_tm);
        if (return_value == RET_SUCCESSFUL)
        {
            returnCode_t test_write = DeviceWrite(dev_ack, (data_t)execution_tm, TM_MAX_SIZE);
            if (test_write != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack, pusExecutionError_t execution_error)
 * @brief       This function send execution non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   dev_ack Device where the ACK TM will be sent
 * @param[in]   execution_error Code explaining why we nack the TC
 * @retval      #RET_INVALID_PARAM if a pointer is null
 * @retval      #RET_ERROR if cannot write into device
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, deviceNo_t dev_ack, pusExecutionError_t execution_error)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        return_value = BuildS1SS8(tc, execution_tm, execution_error);
        if (return_value == RET_SUCCESSFUL)
        {
            returnCode_t test_write = DeviceWrite(dev_ack, (data_t)execution_tm, TM_MAX_SIZE);
            if (test_write != RET_SUCCESSFUL)
            {
                return_value = RET_ERROR;
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CheckCRC(pusTC_t *tc)
 * @brief       Function that verifies a received TC has not been corrupted
 * @param[in]   tc Pointer to the TC variable where we want to check it CRC
 * @retval      #RET_ERROR if the computed CRC is different than the received CRC
 * @retval      #RET_SUCCESSFUL else
 */
static returnCode_t CheckCRC(pusTC_t *tc)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    uint16_t data_size = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length) + 1u;
    pusCRC_t reiceved_crc = (pusCRC_t)(tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 0u] << 8u) +
                            (pusCRC_t)(tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u]);
    pusCRC_t computed_crc = 0u;

    // Function Core
    computed_crc = computeCRC((uint8_t *)tc, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);
    if (computed_crc != reiceved_crc)
    {
        return_value = RET_ERROR;
    }

    return return_value;
}