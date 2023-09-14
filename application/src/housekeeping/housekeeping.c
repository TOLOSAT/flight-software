/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for HK_MGMT Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "housekeeping/housekeeping.h"
#include "fdir.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "pus_tools/tc_management.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/tables_management.h"
#include "services/pus1.h"
#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer);
static void SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm);
static void SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus3_execution_table
 * @brief   Execution table for incomming pus 3 TC 
 * @warning Keys must be ordered from smallest to largest
 */
pusExecutionTable_t g_pus3_execution_table[NB_PUS3_EXECUTION] = 
{
    { BUILD_ROUTING_KEY(OBC_APID, 3u, 5u) , ExecuteS3SS5 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 3u, 6u) , ExecuteS3SS6 , TM_NOT_REQUESTED },
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              HkMain(void *task_dyn_conf)
 * @brief           Main of the HK_MGMT Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void HkMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_pus3_execution_table, NB_PUS3_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        ExecuteTC((pusExecutionTable_t *)&g_pus3_execution_table, NB_PUS3_EXECUTION, TC_PUS3, NO_BUFFER_REF);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}

/**
 * @fn          ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer)
 * @brief       This function executes incoming TC.
 * @param[in]   execution_table Execution table used for treating incoming TC
 * @param[in]   table_size Size of the table
 * @param[in]   input_buffer Buffer from where TC are coming
 * @param[in]   output_buffer Buffer where TM will be sent
 * @return      Nothing
 */
static void ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer)
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
static void SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm)
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
static void SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error)
{
    // Variable Initialisation
    uint32_t write_status;

    // Function Core
    write_status = BuildS1SS8(tc, execution_tm, execution_error);
    CheckErrors(write_status, FDIR_NO_SANCTION);
    write_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
    CheckErrors(write_status, FDIR_NO_SANCTION);
}
