/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <stdio.h>  // cppcheck-suppress misra-c2012-21.6
#include <cmsis_os2.h>

#include "dummy_tasks.h"
#include "tasks.h"
#include "fdir.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define BUFFER_MSG_SIZE     2U      /**< Size of exemple buffer message */
#define UART_MSG_SIZE       1U      /**< Size of exemple uart message */
#define I2C_MSG_SIZE        1U      /**< Size of exemple i2c message */
#define SLAVE_ADDR          0x05    /**< Exemple slave address */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void *task_dyn_conf)
 * @brief   Function that runs the dummy main task.
 * @param   task_dyn_conf Status of the current task
 */
void DummyMainTask(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    rtcTime_t rtc_time;

    // Initialisation
    printf("[#0] Init\n");
    task_status = initPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        RtcGetTime(&rtc_time);
        printf("[%02d:%02d:%03u] Hello\n", rtc_time.minute, rtc_time.second,(unsigned int) rtc_time.millisecond);
        GpioToggle(&led_inst);

        task_status = waitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }

    // In case we accidentally exit from task loop
    osThreadTerminate(NULL);
}