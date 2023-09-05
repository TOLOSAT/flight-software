/**
 * @file    salami.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SALAMI Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "salami/salami.h"
#include "fdir.h"
#include "io_instances.h"
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
 * @fn              SalamiMain(void *task_dyn_conf)
 * @brief           Main of the SALAMI Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void SalamiMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        (void) GpioToggle(&led_inst);
        
        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}