/**
 * @file    iridium.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for IRIDIUM Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "iridium/iridium.h"
#include "kernel.h"
#include "iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumMain(void *task_desc)
 * @brief           Main of the IRIDIUM Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_IRIDIUM_TEXT_SECTION IridiumMain(void *task_desc)
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