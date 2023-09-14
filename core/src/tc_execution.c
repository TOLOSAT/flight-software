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

#include "fdir.h"                   // Must be removed (just improving error management)
#include "conf/buffers_conf.h"      // Must be removed (just doing parameters for function)

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_table Execution table used for treating incoming TC
 * @param[in]   table_size Size of the table
 * @param[in]   input_buffer Buffer from where TC are coming
 * @param[in]   output_buffer Buffer where TM will be sent
 * @return      Nothing
 */
void ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer)
{
    // Variable Initialisation
    pusStatus_t tc_handling_status;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction;

    // First, we check if there is a TC.
    bufferStatus_t buffer_status = ReadBuffer(input_buffer, (bufferMsgAddr_t)&tc, TC_MAX_SIZE);
    if (buffer_status == BUFFER_SUCCESSFUL)
    {
        // Then, we find which TC we have to execute
        pusTMRequested_t tm_requested = 0u;
        uint32_t key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
        tc_handling_status = ExecutionSearch(execution_table, table_size, key, &tm_requested, &ExecutionFunction);
        if (tc_handling_status == PUS_SUCCESSFUL)
        {
            // Now we execute the TC
            pusExecutionError_t error_code = PUS_EXECUTION_NO_ERROR;
            tc_handling_status = ExecutionFunction(&tc, &tm, &error_code);
            if (tc_handling_status == PUS_SUCCESSFUL)
            {
                // Acknowledge TC execution
                SendExecAckTM(&tc, &execution_tm);

                // Check if a specific TM has to be send
                if (tm_requested == TM_REQUESTED)
                {
                    // Send specific TM
                    uint32_t write_status = WriteBuffer(output_buffer, (bufferMsgAddr_t)&tm, TM_MAX_SIZE);
                    CheckErrors(write_status, FDIR_NO_SANCTION);
                }
            }
            else
            {
                // TC Failed to be executed
                SendExecNackTM(&tc, &execution_tm, error_code);
            }
        }
        else
        {
            // TC does not have execution procedure
            SendExecNackTM(&tc, &execution_tm, PUS_EXECUTION_UNAVAILABLE);
        }
    }
    // We reset the TM & TC variables until next call;
    EraseTC(&tc);
    EraseTM(&tm);
    EraseTM(&execution_tm);
}

/**
 * @fn          SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm)
 * @brief       This function send execution acknowledgment TM.
 * @param[in]   tc TC we want to ACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @return      Nothing
 */
void SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm)
{
    // Variable Initialisation
    uint32_t write_status;

    // Function Core
    write_status = BuildS1SS7(tc, execution_tm);
    CheckErrors(write_status, FDIR_NO_SANCTION);
    write_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
    CheckErrors(write_status, FDIR_NO_SANCTION);
}

/**
 * @fn          SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
 * @brief       This function send execution non acknowledgment TM.
 * @param[in]   tc TC we want to NACK
 * @param[out]  execution_tm Pointer to the execution TM
 * @param[in]   acceptance_error Code explaining why we nack the TC
 * @return      Nothing
 */
void SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
{
    // Variable Initialisation
    uint32_t write_status;

    // Function Core
    write_status = BuildS1SS8(tc, execution_tm, execution_error);
    CheckErrors(write_status, FDIR_NO_SANCTION);
    write_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
    CheckErrors(write_status, FDIR_NO_SANCTION);
}
