/**
 * @file    aocs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for AOCS Task
 * @date    28/06/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "aocs/aocs.h"
#include "core_basics.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              AocsMain(void *task_dyn_conf)
 * @brief           Main of the aocs Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_AOCS_TEXT_SECTION AocsMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}