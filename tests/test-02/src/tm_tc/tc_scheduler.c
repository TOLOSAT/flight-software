/**
 * @file tc_scheduler.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_SCHEDULER Task
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tc_scheduler.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

#define TASK_NAME       "TC_SCHEDULER"      /**< Current Task Name */
#define TASK_ID         (TC_SCHEDULER_TASK) /**< Current Task ID */

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      TcSchedulerMain(void *task_dyn_conf)
 * @brief   Main of the TC_SCHEDULER Task
 * @param   task_dyn_conf Status of the current task
 */
void TcSchedulerMain(void *task_dyn_conf)
{
    // Variable Initialisation

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}