/**
 * @file    housekeeping.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for HK_MGMT Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "housekeeping/housekeeping.h"
#include "fdir.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"
#include "tc_execution.h"
#include "services/pus1.h"
#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

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
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *) &g_pus3_execution_table, NB_PUS3_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS3, NO_BUFFER_REF, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_pus3_execution_table, NB_PUS3_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
