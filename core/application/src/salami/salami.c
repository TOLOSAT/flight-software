/**
 * @file salami.c
 * @author Merlin Kooshmanian
 * @brief Source file for SALAMI Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "salami/salami.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define TASK_NAME       "SALAMI"        /**< Current Task Name */
#define TASK_ID         (SALAMI_TASK)   /**< Current Task ID */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn              SalamiMain(void *task_dyn_conf)
 * @brief           Main of the SALAMI Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void SalamiMain(void *task_dyn_conf)
{
    // Variable Initialisation

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        printf("["TASK_NAME"] Hello\n");
        GpioToggle(&led_inst);
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}