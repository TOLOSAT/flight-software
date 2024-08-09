/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "core_basics.h"
#include "conf/platform_conf.h"

/***************************** Macros Definitions ****************************/

#define BUFFER_MSG_SIZE     2U      /**< Size of exemple buffer message */
#define UART_MSG_SIZE       1U      /**< Size of exemple uart message */
#define I2C_MSG_SIZE        1U      /**< Size of exemple i2c message */
#define SLAVE_ADDR          0x05    /**< Exemple slave address */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask01(void *task_desc)
 * @brief   Function that runs the dummy main task.
 * @param   task_desc Descriptor of the current task
 */
void DummyMainTask01(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        ConsolePrint("[#1] Hello\n");
        (void)GpioToggle(&user_led_inst);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn      DummyMainTask02(void *task_desc)
 * @brief   Function that runs the dummy main task.
 * @param   task_desc Descriptor of the current task
 */
void DummyMainTask02(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    ConsolePrint("[#2] Init\n");
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        ConsolePrint("[#2] Hello\n");

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}