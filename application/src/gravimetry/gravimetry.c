/**
 * @file    gravimetry.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GRAVIMETRY Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "gravimetry/gravimetry.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              GravimetryMain(void *task_desc)
 * @brief           Main of the GRAVIMETRY Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_GRAVIMETRY_TEXT_SECTION GravimetryMain(void *task_desc)
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