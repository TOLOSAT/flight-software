/**
 * @file    power.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for POWER Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "power/power.h"
#include "fdir.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              PowerMain(void *task_dyn_conf)
 * @brief           Main of the power Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void PowerMain(void *task_dyn_conf)
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