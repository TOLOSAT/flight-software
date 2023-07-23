/**
 * @file dummy_tasks.c
 * @author Merlin Kooshmanian
 * @brief Source file with dummy tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <cmsis_os2.h>

#include "tasks.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define BUFFER_MSG_SIZE     2U      /**< Size of exemple buffer message */
#define UART_MSG_SIZE       1U      /**< Size of exemple uart message */
#define I2C_MSG_SIZE        1U      /**< Size of exemple i2c message */
#define SLAVE_ADDR          0x05    /**< Exemple slave address */

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;
extern uartInst_t uart_tmtc_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void *argument)
 * @brief   Function that runs the dummy main task.
 * @param   task_dyn_conf Status of the current task
 */
void DummyMainTask(void *task_dyn_conf)
{
    // Variable Initialisation
    rtcTime_t rtc_time;

    // Initialisation
    printf("[#0] Init\n");
    initPeriodicWait(task_dyn_conf);

    // Function Core
    while (1)
    {
        RtcGetTime(&rtc_time);
        printf("[%02d:%02d] Hello\n", rtc_time.minute, rtc_time.second);
        GpioToggle(&led_inst);
        waitUntilNextPeriod(task_dyn_conf);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}