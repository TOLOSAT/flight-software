/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_scheduler.h"
#include "core_basics.h"
#include "tc_execution.h"
#include "services/pus1.h"
#include "services/pus11.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static pusStatus_t ProcessDelayedTC(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus11_execution_table
 * @brief   Execution table for incomming pus 11 TC 
 * @warning Keys must be ordered from smallest to largest
 */
pusExecutionTable_t g_pus11_execution_table[NB_PUS11_EXECUTION] = 
{
    { BUILD_ROUTING_KEY(OBC_APID, 11u, 1u) , ExecuteS11SS1 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 11u, 2u) , ExecuteS11SS2 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 11u, 3u) , ExecuteS11SS3 , TM_NOT_REQUESTED },
    { BUILD_ROUTING_KEY(OBC_APID, 11u, 4u) , ExecuteS11SS4 , TM_NOT_REQUESTED },
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              TcSchedulerMain(void *task_dyn_conf)
 * @brief           Main of the TC_SCHEDULER Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void TcSchedulerMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_pus11_execution_table, NB_PUS11_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPus11();
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS11, NO_BUFFER_REF, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_pus11_execution_table, NB_PUS11_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Process delayed TC
        task_status = ProcessDelayedTC();
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn      ProcessDelayedTC(void)
 * @brief   Function that get delayed tc and transfer it to tc receiver
 * @retval  #PUS_ERROR if an error occured (from WriteBuffer or GetDelayedTC)
 * @retval  #PUS_SUCCESSFUL else
 */
static pusStatus_t ProcessDelayedTC(void)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusStatus_t test_pus11;
    pusTC_t delayed_tc = {0};

    // Get delayed TC if there is any
    test_pus11 = GetDelayedTC(&delayed_tc);
    if (test_pus11 == PUS_SUCCESSFUL)
    {
        // Delayed TC available, send it to TC receiver
        bufferStatus_t test_write = WriteBuffer(TC_DELAYED, (bufferMsgAddr_t)&delayed_tc, TC_MAX_SIZE);
        if (test_write != BUFFER_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
        }
    }
    else if (test_pus11 == PUS_NOT_AVAILABLE)
    {
        // No delayed TC available
        return_value = PUS_SUCCESSFUL;
    }
    else
    {
        // An error occured
        return_value = PUS_ERROR;
    }

    return return_value;
}