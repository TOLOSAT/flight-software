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
#include "core_basics.h"
#include "iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IridiumMain(void *task_dyn_conf)
 * @brief           Main of the IRIDIUM Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_IRIDIUM_TEXT_SECTION IridiumMain(void *task_dyn_conf)
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