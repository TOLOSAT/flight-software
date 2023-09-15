/**
 * @file    tc_scheduler.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "tm_tc/tc_scheduler.h"
#include "fdir.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "tc_execution.h"
#include "services/pus1.h"
#include "services/pus11.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_pus11_execution_table
 * @brief   Execution table for incomming pus 11 TC 
 * @warning Keys must be ordered from smallest to largest
 */
pusExecutionTable_t g_pus11_execution_table[NB_PUS11_EXECUTION] = 
{
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
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_pus11_execution_table, NB_PUS11_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS11, NO_BUFFER_REF, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_pus11_execution_table, NB_PUS11_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}
