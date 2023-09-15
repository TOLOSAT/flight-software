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
 * @fn          ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, tcExecutionBasicBuffers_t basic_buffers)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_table Execution table used for treating incoming TC
 * @param[in]   table_size Size of the table
 * @param[in]   input_buffer Buffer from where TC are coming
 * @param[in]   output_buffer Buffer where TM will be sent
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
        // We reset the TM & TC variables until next call;
        EraseTC(&tc);
        EraseTM(&tm);
        EraseTM(&execution_tm);
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
 * @return      Nothing
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
 * @param[in]   acceptance_error Code explaining why we nack the TC
 * @return      Nothing
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
