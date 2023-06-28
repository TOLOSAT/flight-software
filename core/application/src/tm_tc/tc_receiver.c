/**
 * @file tc_receiver.c
 * @author Merlin Kooshmanian
 * @brief Source file for TC_RECEIVER Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tm_tc/tc_receiver.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

#define TASK_NAME       "TC_RECEIVER"
#define TASK_ID         (TC_RECEIVER_TASK)

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      TcReceiverMain(void *current_status)
 * @brief   Main of the TC_RECEIVER Task
 * @param   current_status Status of the current task
 */
void TcReceiverMain(void *current_status)
{
    // Variable Initialisation

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(current_status);

    // Function Core
    while (1)
    {
        printf("["TASK_NAME"] Hello\n");
        waitUntilNextPeriod(current_status);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}