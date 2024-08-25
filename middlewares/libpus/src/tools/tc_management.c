/**
 * @file    tc_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC management
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "pus.h"
#include "services/pus1.h"
#include "endianness.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, bufferNo_t ack_buffer);
static pusStatus_t SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, bufferNo_t ack_buffer, pusAcceptanceError_t acceptance_error);
static pusStatus_t SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm, bufferNo_t ack_buffer);
static pusStatus_t SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, bufferNo_t ack_buffer, pusExecutionError_t execution_error);
static pusStatus_t CheckCRC(pusTC_t *tc);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ProcessNewTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, bufferNo_t ack_buffer)
 * @brief       Function that will process a new incoming TC and routes it toward it's corresponding task
 * @param[in]   routing_table Routing table used for route TC to other tasks
 * @param[in]   table_size Size of the routing TC
 * @param[in]   tc TC that is processed
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @retval      #PUS_INVALID_PARAM if a pointer is a null pointer or routing table size is null
 * @retval      #PUS_ERROR if cannot format TC
 * @retval      #PUS_ERROR if cannot write TC into it's buffer
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION ProcessNewTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, bufferNo_t ack_buffer)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusTM_t acceptance_tm = {0};
    pusAcceptanceError_t acceptance_error = PUS_ACCEPTANCE_NO_ERROR;
    

    // Function Core
    if ((routing_table != NULL) && (table_size != 0u) && (tc != NULL))
    {
        // First, we check the validity of the TC.
        return_value = CheckTCValidity(tc, &acceptance_error);
        if (return_value == PUS_SUCCESSFUL)
        {
            // If TC is valid, we format the TC because of endianness.
            return_value = FormatTC(tc);
            if (return_value == PUS_SUCCESSFUL)
            {
                // Then, we route the TC toward the task that will execute it.
                bufferNo_t route = 0u;
                uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc->spp_header.packet_id), tc->tc_header.service, tc->tc_header.subservice);
                return_value = RouteSearch((pusRoutingTable_t *)routing_table, table_size, key, &route);
                if (return_value == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC
                    (void)SendAcptAckTM(tc, &acceptance_tm, ack_buffer);

                    // Send TC to the task that will execute it
                    coreStatus_t test_buffer = WriteBuffer(route, (bufferMsgAddr_t)tc, TC_MAX_SIZE);
                    if (test_buffer != CORE_SUCCESSFUL)
                    {
                        return_value = PUS_ERROR;
                    }
                }
                else
                {
                    // Bad routing so TC non acknowleded
                    (void)SendAcptNackTM(tc, &acceptance_tm, ack_buffer, PUS_ACCEPTANCE_INVALID_ROUTE);
                }
            }
            else
            {
                // Can't format so TC non acknowleded
                (void)SendAcptNackTM(tc, &acceptance_tm, ack_buffer, PUS_ACCEPTANCE_CANT_FORMAT);
            }
        }
        else
        {
            // Invalid TC, TC will be non-acknowledged.
            (void)SendAcptNackTM(tc, &acceptance_tm, ack_buffer, acceptance_error);
        }

        // We erase TC for next call;
        EraseTC(tc);
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferNo_t tc_buffer, bufferNo_t tm_buffer, bufferNo_t ack_buffer)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_table Execution table used for treating incoming TC
 * @param[in]   table_size Size of the table
 * @param[in]   tc_buffer Buffer where the TC come from
 * @param[in]   tm_buffer Buffer where to put the TM
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @return      Nothing
 */
pusStatus_t IN_PUS_TEXT_SECTION ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferNo_t tc_buffer, bufferNo_t tm_buffer, bufferNo_t ack_buffer)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction = NULL; // cppcheck-suppress [misra-c2012-17.7,unmatchedSuppression]; False positive because ExecutionFunction is declared and not called

    // Function core
    if ((execution_table != NULL) && (table_size != 0u))
    {
        // First, we check if there is a TC.
        coreStatus_t test_buffer = ReadBuffer(tc_buffer, (bufferMsgAddr_t)&tc, TC_MAX_SIZE);
        if (test_buffer == CORE_SUCCESSFUL)
        {
            // Then, we find which TC we have to execute
            pusTMRequested_t tm_requested = 0u;
            uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
            return_value = ExecutionSearch(execution_table, table_size, key, &tm_requested, &ExecutionFunction);
            if (return_value == PUS_SUCCESSFUL)
            {
                // Now we execute the TC
                pusExecutionError_t error_code = PUS_EXECUTION_FAILED;
                return_value = ExecutionFunction(&tc, &tm, &error_code);
                if (return_value == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC execution
                    (void)SendExecAckTM(&tc, &execution_tm, ack_buffer);

                    // Check if a specific TM has to be send
                    if (tm_requested == TM_REQUESTED)
                    {
                        // Send specific TM
                        test_buffer = WriteBuffer(tm_buffer, (bufferMsgAddr_t)&tm, TM_MAX_SIZE);
                        if (test_buffer != CORE_SUCCESSFUL)
                        {
                            return_value = PUS_ERROR;
                        }
                    }
                }
                else
                {
                    // TC Failed to be executed
                    (void)SendExecNackTM(&tc, &execution_tm, ack_buffer, error_code);
                }
            }
            else
            {
                // TC does not have execution procedure
                (void)SendExecNackTM(&tc, &execution_tm, ack_buffer, PUS_EXECUTION_UNAVAILABLE);
            }
        }
        else
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              FormatTC(pusTC_t *tc)
 * @brief           Function that format TC the right way
 * @param[in,out]   tc Pointer to the TC we want to format
 * @retval          #PUS_INVALID_PARAM if tc is null pointer
 * @retval          #PUS_SUCCESSFUL else
 *
 * As we've done a silly memcpy with the uart driver, the
 * TC fields don't have the right endianness, or aren't in
 * the right place.
 */
pusStatus_t IN_PUS_TEXT_SECTION FormatTC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

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
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
 * @brief       Function that verifies if TC is valid (right version, type, size)
 * @param[in]   tc Pointer to the TC variable where we want to verify it validity.
 * @param[out]  error Pointer to pass error type to TM(1,2)
 * @retval      #PUS_ERROR if
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t IN_PUS_TEXT_SECTION CheckTCValidity(pusTC_t *tc, pusAcceptanceError_t *error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
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
                    if (CheckCRC(tc) != PUS_SUCCESSFUL)
                    {
                        return_value = PUS_ERROR;
                        *error = PUS_ACCEPTANCE_INVALID_CRC;
                    }
                }
                else
                {
                    return_value = PUS_ERROR;
                    *error = PUS_ACCEPTANCE_INVALID_FORMAT;
                }
            }
            else
            {
                return_value = PUS_ERROR;
                *error = PUS_ACCEPTANCE_INVALID_FORMAT;
            }
        }
        else
        {
            return_value = PUS_ERROR;
            *error = PUS_ACCEPTANCE_INVALID_FORMAT;
        }
    }
    else
    {
        return_value = PUS_ERROR;
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
void IN_PUS_TEXT_SECTION EraseTC(pusTC_t *tc)
{
    // Function Core
    (void)memset(tc, 0u, TC_MAX_SIZE);
}

/**
 * @fn          SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, bufferNo_t ack_buffer)
 * @brief       This function send acceptance acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if cannot write into buffer
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SendAcptAckTM(const pusTC_t *tc, pusTM_t *acceptance_tm, bufferNo_t ack_buffer)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        return_value = BuildS1SS1(tc, acceptance_tm);
        if (return_value == PUS_SUCCESSFUL)
        {
            coreStatus_t test_buffer = WriteBuffer(ack_buffer, (bufferMsgAddr_t)acceptance_tm, TM_MAX_SIZE);
            if (test_buffer != CORE_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error)
 * @brief       This function send acceptance non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @param[in]   acceptance_error Code explaining why we nack the TC
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if cannot write into buffer
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SendAcptNackTM(const pusTC_t *tc, pusTM_t *acceptance_tm, bufferNo_t ack_buffer, pusAcceptanceError_t acceptance_error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        return_value = BuildS1SS2(tc, acceptance_tm, acceptance_error);
        if (return_value == PUS_SUCCESSFUL)
        {
            coreStatus_t test_buffer = WriteBuffer(ack_buffer, (bufferMsgAddr_t)acceptance_tm, TM_MAX_SIZE);
            if (test_buffer != CORE_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm)
 * @brief       This function send execution acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if cannot write into buffer
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SendExecAckTM(const pusTC_t *tc, pusTM_t *execution_tm, bufferNo_t ack_buffer)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        return_value = BuildS1SS7(tc, execution_tm);
        if (return_value == PUS_SUCCESSFUL)
        {
            coreStatus_t test_buffer = WriteBuffer(ack_buffer, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
            if (test_buffer != CORE_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
 * @brief       This function send execution non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   ack_buffer Buffer where to put the ACK TM
 * @param[in]   execution_error Code explaining why we nack the TC
 * @retval      #PUS_INVALID_PARAM if a pointer is null
 * @retval      #PUS_ERROR if cannot write into buffer
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION SendExecNackTM(const pusTC_t *tc, pusTM_t *execution_tm, bufferNo_t ack_buffer, pusExecutionError_t execution_error)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        return_value = BuildS1SS8(tc, execution_tm, execution_error);
        if (return_value == PUS_SUCCESSFUL)
        {
            coreStatus_t test_buffer = WriteBuffer(ack_buffer, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
            if (test_buffer != CORE_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CheckCRC(pusTC_t *tc)
 * @brief       Function that verifies a received TC has not been corrupted
 * @param[in]   tc Pointer to the TC variable where we want to check it CRC
 * @retval      #PUS_ERROR if the computed CRC is different than the received CRC
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t IN_PUS_TEXT_SECTION CheckCRC(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    uint16_t data_size = HALF_WORD_BYTE_SWAP(tc->spp_header.packet_data_length) + 1u;
    pusCRC_t reiceved_crc = (pusCRC_t)(tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 0u] << 8u) +
                            (pusCRC_t)(tc->data[data_size - TC_HEADER_SIZE - CRC_TRAILER_SIZE + 1u]);
    pusCRC_t computed_crc = 0u;

    // Function Core
    computed_crc = computeCRC((uint8_t *)tc, data_size + SPP_HEADER_SIZE - CRC_TRAILER_SIZE);
    if (computed_crc != reiceved_crc)
    {
        return_value = PUS_ERROR;
    }

    return return_value;
}