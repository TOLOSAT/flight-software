/**
 * @file carne.c
 * @author Merlin Kooshmanian
 * @brief Source file for CARNE Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "carne/carne.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define TASK_NAME       "CARNE"         /**< Current Task Name */
#define TASK_ID         (CARNE_TASK)    /**< Current Task ID */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              CarneMain(void *task_dyn_conf)
 * @brief           Main of the CARNE Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void CarneMain(void *task_dyn_conf)
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