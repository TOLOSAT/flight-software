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
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              AocsMain(void *task_desc)
 * @brief           Main of the aocs Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_AOCS_TEXT_SECTION AocsMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}