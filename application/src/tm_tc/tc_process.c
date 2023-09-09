/**
 * @file    tc_process.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_PROCESS Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "tm_tc/tc_process.h"
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
#include "services/pus6.h"
#include "services/pus9.h"
#include "services/pus17.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm);
static void SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_tc_execution_table
 * @brief   Execution table for incomming TC 
 * @warning Keys must be ordered from smallest to largest
 */
pusExecutionTable_t g_tc_execution_table[NB_EXECUTION] = 
{
    {.key = BUILD_ROUTING_KEY(OBC_APID,  6u,   1u) , ExecuteS6SS1  , TM_NOT_REQUESTED },
    {.key = BUILD_ROUTING_KEY(OBC_APID,  6u,   3u) , ExecuteS6SS3  , TM_REQUESTED     },
    {.key = BUILD_ROUTING_KEY(OBC_APID,  9u, 128u) , ExecuteS9SS128, TM_NOT_REQUESTED },
    {.key = BUILD_ROUTING_KEY(OBC_APID, 17u,   1u) , ExecuteS17SS1 , TM_REQUESTED     },
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcProcessMain(void *task_dyn_conf)
 * @brief           Main of the TC_PROCESS Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void TcProcessMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    pusStatus_t tc_handling_status;
    uint32_t key;
    pusTC_t tc = {0};
    pusTM_t tm = {0};
    pusTM_t execution_tm = {0};
    pusExecutionFunctionPtr_t ExecutionFunction;

    // Initialisation
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_tc_execution_table, NB_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // First, we check if there is a TC.
        bufferStatus_t buffer_status = ReadBuffer(TC_NORMAL, (bufferMsgAddr_t) &tc, TC_MAX_SIZE);
        if(buffer_status == BUFFER_SUCCESSFUL)
        {
            // Then, we find which TC we have to execute
            pusTMRequested_t tm_requested = 0u;
            key = BUILD_ROUTING_KEY((APID_MASK & tc.spp_header.packet_id), tc.tc_header.service, tc.tc_header.subservice);
            tc_handling_status = ExecutionSearch((pusExecutionTable_t *) &g_tc_execution_table, NB_EXECUTION, key, &tm_requested, &ExecutionFunction);
            if(tc_handling_status ==  PUS_SUCCESSFUL)
            {
                // Now we execute the TC
                tc_handling_status = ExecutionFunction(&tc, &tm);
                if(tc_handling_status == PUS_SUCCESSFUL)
                {
                    // Acknowledge TC execution
                    SendExecAckTM(&tc, &execution_tm);

                    // Check if a specific TM has to be send 
                    if(tm_requested == TM_REQUESTED)
                    {
                        // Send specific TM
                        task_status = WriteBuffer(TM_NORMAL, (bufferMsgAddr_t) &tm, TM_MAX_SIZE);
                        CheckErrors(task_status, FDIR_NO_SANCTION);
                    }
                }
                else
                {
                    // TC Failed to be executed
                    SendExecNackTM(&tc, &execution_tm, PUS_EXECUTION_FAILED);
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

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
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
    uint32_t task_status;

    // Function Core
    task_status = BuildS1SS7(tc, execution_tm);
    CheckErrors(task_status, FDIR_NO_SANCTION);
    task_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
    CheckErrors(task_status, FDIR_NO_SANCTION);
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
    uint32_t task_status;

    // Function Core
    task_status = BuildS1SS8(tc, execution_tm, execution_error);
    CheckErrors(task_status, FDIR_NO_SANCTION);
    task_status = WriteBuffer(TM_PUS1, (bufferMsgAddr_t)execution_tm, TM_MAX_SIZE);
    CheckErrors(task_status, FDIR_NO_SANCTION);
}
