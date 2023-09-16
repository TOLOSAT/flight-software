/**
 * @file    tc_execution.c
 * @author  Merlin Kooshmanian
 * @brief   Source file including function for generic tc execution
 * @date    14/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tc_execution.h"
#include "services/pus1.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ProcessTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, bufferRef_t ack_buffer)
 * @brief       Function that will process a new incoming TC and routes it toward it's corresponding task
 * @param[in]   routing_table Routing table used for route TC to other tasks
 * @param[in]   table_size Size of the routing TC
 * @param[in]   tc TC that is processed
 * @param[in]   ack_buffer Buffer for ACK TM
 * @retval      #PUS_INVALID_PARAM if a pointer is a null pointer or routing table size is null
 * @retval      #PUS_ERROR if cannot format TC
 * @retval      #PUS_ERROR if cannot write TC into it's buffer
 * @retval      #PUS_SUCCESSFUL else
 */
tcExecutionStatus_t ProcessTC(pusRoutingTable_t *routing_table, pusTableSize_t table_size, pusTC_t *tc, bufferRef_t ack_buffer)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;
    pusTM_t acceptance_tm = {0};
    pusAcceptanceError_t acceptance_error = PUS_ACCEPTANCE_NO_ERROR;
    pusStatus_t test_val;

    // Function Core
    if ((routing_table != NULL) && (table_size != 0u) && (tc != NULL))
    {
        // First, we check the validity of the TC.
        test_val = CheckTCValidity(tc, &acceptance_error);
        if (test_val == PUS_SUCCESSFUL)
        {
            // If TC is valid, we format the TC because of endianness.
            test_val = FormatTC(tc);
            if (test_val == PUS_SUCCESSFUL)
            {
                // Then, we route the TC toward the task that will execute it.
                bufferRef_t route = 0u;
                uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc->spp_header.packet_id), tc->tc_header.service, tc->tc_header.subservice);
                test_val = RouteSearch((pusRoutingTable_t *)routing_table, table_size, key, &route);
                if (test_val == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC
                    (void)SendAcptAckTM(tc, &acceptance_tm, ack_buffer);

                    // Send TC to the task that will execute it
                    bufferStatus_t write_status = WriteBuffer(route, (bufferMsgAddr_t)tc, TC_MAX_SIZE);
                    if (write_status != BUFFER_SUCCESSFUL)
                    {
                        return_value = TC_EXECUTION_ERROR;
                    }
                }
                else
                {
                    // Bad routing so TC non acknowleded
                    (void)SendAcptNackTM(tc, &acceptance_tm, PUS_ACCEPTANCE_INVALID_ROUTE, ack_buffer);
                }
            }
            else
            {
                return_value = TC_EXECUTION_ERROR;
            }
        }
        else
        {
            // Invalid TC, TC will be non-acknowledged.
            (void)SendAcptNackTM(tc, &acceptance_tm, acceptance_error, ack_buffer);
        }

        // We erase TC for next call;
        EraseTC(tc);
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, tcExecutionBasicBuffers_t basic_buffers)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_table Execution table used for treating incoming TC
 * @param[in]   table_size Size of the table
 * @param[in]   basic_buffers Struct indicating basic buffer (tc, tm and ack_tm)
 * @return      Nothing
 */
tcExecutionStatus_t ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, tcExecutionBasicBuffers_t basic_buffers)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;
    pusStatus_t tc_handling_status;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction;

    // Function core
    if ((execution_table != NULL) && (table_size != 0u))
    {
        // First, we check if there is a TC.
        bufferStatus_t buffer_status = ReadBuffer(basic_buffers.tc_buffer, (bufferMsgAddr_t)&tc, TC_MAX_SIZE);
        if (buffer_status == BUFFER_SUCCESSFUL)
        {
            // Then, we find which TC we have to execute
            pusTMRequested_t tm_requested = 0u;
            uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
            tc_handling_status = ExecutionSearch(execution_table, table_size, key, &tm_requested, &ExecutionFunction);
            if (tc_handling_status == PUS_SUCCESSFUL)
            {
                // Now we execute the TC
                pusExecutionError_t error_code = PUS_EXECUTION_FAILED;
                tc_handling_status = ExecutionFunction(&tc, &tm, &error_code);
                if (tc_handling_status == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC execution
                    (void)SendExecAckTM(&tc, &execution_tm, basic_buffers.ack_buffer);

                    // Check if a specific TM has to be send
                    if (tm_requested == TM_REQUESTED)
                    {
                        // Send specific TM
                        buffer_status = WriteBuffer(basic_buffers.tm_buffer, (bufferMsgAddr_t)&tm, TM_MAX_SIZE);
                        if (buffer_status != BUFFER_SUCCESSFUL)
                        {
                            return_value = TC_EXECUTION_ERROR;
                        }
                    }
                }
                else
                {
                    // TC Failed to be executed
                    (void)SendExecNackTM(&tc, &execution_tm, error_code, basic_buffers.ack_buffer);
                    return_value = TC_EXECUTION_ERROR;
                }
            }
            else
            {
                // TC does not have execution procedure
                (void)SendExecNackTM(&tc, &execution_tm, PUS_EXECUTION_UNAVAILABLE, basic_buffers.ack_buffer);
                return_value = TC_EXECUTION_ERROR;
            }
        }
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendAcptAckTM(pusTC_t *tc, pusTM_t *acceptance_tm, bufferRef_t ack_buffer)
 * @brief       This function send acceptance acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   ack_buffer Buffer to put ack tm in
 * @retval      #TC_EXECUTION_INVALID_PARAM if a pointer is null
 * @retval      #TC_EXECUTION_ERROR if cannot write into buffer
 * @retval      #TC_EXECUTION_SUCCESSFUL else
 */
tcExecutionStatus_t SendAcptAckTM(pusTC_t *tc, pusTM_t *acceptance_tm, bufferRef_t ack_buffer)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        uint32_t send_ack_status = BuildS1SS1(tc, acceptance_tm);
        if (send_ack_status == 0u)
        {
            send_ack_status = WriteBuffer(ack_buffer, (bufferMsgAddr_t)acceptance_tm, TM_MAX_SIZE);
            if (send_ack_status != 0u)
            {
                return_value = TC_EXECUTION_ERROR;
            }
        }
        else
        {
            return_value = TC_EXECUTION_ERROR;
        }
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendAcptNackTM(pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error, bufferRef_t ack_buffer)
 * @brief       This function send acceptance non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  acceptance_tm Pointer to the acceptance TM
 * @param[in]   acceptance_error Code explaining why we nack the TC
 * @param[in]   ack_buffer Buffer to put ack tm in
 * @retval      #TC_EXECUTION_INVALID_PARAM if a pointer is null
 * @retval      #TC_EXECUTION_ERROR if cannot write into buffer
 * @retval      #TC_EXECUTION_SUCCESSFUL else
 */
tcExecutionStatus_t SendAcptNackTM(pusTC_t *tc, pusTM_t *acceptance_tm, pusAcceptanceError_t acceptance_error, bufferRef_t ack_buffer)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (acceptance_tm != NULL))
    {
        uint32_t send_ack_status = BuildS1SS2(tc, acceptance_tm, acceptance_error);
        if (send_ack_status == 0u)
        {
            send_ack_status = WriteBuffer(ack_buffer, (bufferMsgAddr_t)acceptance_tm, TM_MAX_SIZE);
            if (send_ack_status != 0u)
            {
                return_value = TC_EXECUTION_ERROR;
            }
        }
        else
        {
            return_value = TC_EXECUTION_ERROR;
        }
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm, bufferRef_t ack_buffer)
 * @brief       This function send execution acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   ack_buffer Buffer to put ack tm in
 * @retval      #TC_EXECUTION_INVALID_PARAM if a pointer is null
 * @retval      #TC_EXECUTION_ERROR if cannot write into buffer
 * @retval      #TC_EXECUTION_SUCCESSFUL else
 */
tcExecutionStatus_t SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm, bufferRef_t ack_buffer)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        uint32_t send_ack_status = BuildS1SS7(tc, execution_tm);
        if (send_ack_status == 0u)
        {
            send_ack_status = WriteBuffer(ack_buffer, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
            if (send_ack_status != 0u)
            {
                return_value = TC_EXECUTION_ERROR;
            }
        }
        else
        {
            return_value = TC_EXECUTION_ERROR;
        }
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error, bufferRef_t ack_buffer)
 * @brief       This function send execution non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   execution_error Code explaining why we nack the TC
 * @param[in]   ack_buffer Buffer to put ack tm in
 * @retval      #TC_EXECUTION_INVALID_PARAM if a pointer is null
 * @retval      #TC_EXECUTION_ERROR if cannot write into buffer
 * @retval      #TC_EXECUTION_SUCCESSFUL else
 */
tcExecutionStatus_t SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error, bufferRef_t ack_buffer)
{
    // Variable Initialisation
    tcExecutionStatus_t return_value = TC_EXECUTION_SUCCESSFUL;

    // Function Core
    if ((tc != NULL) && (execution_tm != NULL))
    {
        uint32_t send_nack_status = BuildS1SS8(tc, execution_tm, execution_error);
        if (send_nack_status == 0u)
        {
            send_nack_status = WriteBuffer(ack_buffer, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
            if (send_nack_status != 0u)
            {
                return_value = TC_EXECUTION_ERROR;
            }
        }
        else
        {
            return_value = TC_EXECUTION_ERROR;
        }
    }
    else
    {
        return_value = TC_EXECUTION_INVALID_PARAM;
    }

    return return_value;
}
