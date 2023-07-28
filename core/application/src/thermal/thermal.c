/**
 * @file thermal.c
 * @author Merlin Kooshmanian
 * @brief Source file for THERMAL Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "thermal/thermal.h"
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
 * @fn              ThermalMain(void *task_dyn_conf)
 * @brief           Main of the thermal Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void ThermalMain(void *task_dyn_conf)
{
    // Variable Initialisation

    // Initialisation
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}