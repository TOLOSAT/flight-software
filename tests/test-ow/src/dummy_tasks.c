/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <stdio.h>

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
extern owInst_t one_wire_inst;

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

    // Initialisation
    printf("[#1] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        printf("[#1] Hello\n");
        GpioToggle(&led_inst);

        halIoCtlCmd_t ow_init = {OW_IOCTL_INIT_CONNECTION, 0u, NULL};
        OwIoctl(&one_wire_inst, ow_init);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
