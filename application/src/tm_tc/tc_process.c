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
#include "tc_execution.h"
#include "services/pus1.h"
#include "services/pus6.h"
#include "services/pus9.h"
#include "services/pus17.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_normal_execution_table
 * @brief   Execution table for incomming TC
 * @warning Keys must be ordered from smallest to largest
 */
pusExecutionTable_t g_normal_execution_table[NB_NORMAL_EXECUTION] =
{
    {BUILD_ROUTING_KEY(OBC_APID, 6u, 1u)   , ExecuteS6SS1   , TM_NOT_REQUESTED },
    {BUILD_ROUTING_KEY(OBC_APID, 6u, 3u)   , ExecuteS6SS3   , TM_REQUESTED     },
    {BUILD_ROUTING_KEY(OBC_APID, 9u, 128u) , ExecuteS9SS128 , TM_NOT_REQUESTED },
    {BUILD_ROUTING_KEY(OBC_APID, 17u, 1u)  , ExecuteS17SS1  , TM_REQUESTED     },
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

    // Initialisation
    task_status = CheckExecutionTable((pusExecutionTable_t *)&g_normal_execution_table, NB_NORMAL_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        const tcExecutionBasicBuffers_t basic_buffers = {TC_NORMAL, TM_NORMAL, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_normal_execution_table, NB_NORMAL_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}
