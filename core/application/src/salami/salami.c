/**
 * @file salami.c
 * @author Merlin Kooshmanian
 * @brief Source file for SALAMI Task
 * @date 28/06/2023
 * 
 * Last Update : 28/06/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "salami/salami.h"
#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

#define TASK_NAME       "SALAMI"        /**< Current Task Name */
#define TASK_ID         (SALAMI_TASK)   /**< Current Task ID */

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern gpioInst_t led_inst;

/************************* Functions Definitions *****************************/

/**
 * @fn      SalamiMain(void *current_status)
 * @brief   Main of the SALAMI Task
 * @param   current_status Status of the current task
 */
void SalamiMain(void *current_status)
{
    // Variable Initialisation

    // Initialisation
    printf("["TASK_NAME"] Init\n");
    initPeriodicWait(current_status);

    // Function Core
    while (1)
    {
        printf("["TASK_NAME"] Hello\n");
        GpioToggle(&led_inst);
        waitUntilNextPeriod(current_status);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}