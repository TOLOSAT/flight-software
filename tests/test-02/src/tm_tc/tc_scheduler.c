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
 * @fn      TcSchedulerMain(void *current_status)
 * @brief   Main of the TC_SCHEDULER Task
 * @param   current_status Status of the current task
 */
void TcSchedulerMain(void *current_status)
{
    // Variable Initialisation

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(current_status);

    // Function Core
    while (1)
    {
        waitUntilNextPeriod(current_status);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}