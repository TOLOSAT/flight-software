/**
 * @file    tc_process.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TC_PROCESS Task
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tc_process.h"
#include "core.h"
#include "pus.h"
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
 * @fn              TcProcessMain(void *task_desc)
 * @brief           Main of the TC_PROCESS Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void TcProcessMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = CheckExecutionTable((pusExecutionTable_t *)&g_normal_execution_table, NB_NORMAL_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Execute incoming TC
        task_status = ExecuteTC((pusExecutionTable_t *)&g_normal_execution_table, NB_NORMAL_EXECUTION, TC_NORMAL, TM_NORMAL, TM_PUS1);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
