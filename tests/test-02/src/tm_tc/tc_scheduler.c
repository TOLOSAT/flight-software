/**
 * @file tc_scheduler.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_SCHEDULER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <cmsis_os2.h>

#include "tm_tc/tc_scheduler.h"
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
 * @fn      TcSchedulerMain(void *task_dyn_conf)
 * @brief   Main of the TC_SCHEDULER Task
 * @param   task_dyn_conf Status of the current task
 */
void TcSchedulerMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, ERROR_HANDLER);

    // Function Core
    while (1)
    {
        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}