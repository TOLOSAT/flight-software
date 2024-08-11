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
#include "core.h"

/***************************** Macros Definitions ****************************/

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
    // Unused
    (void)(task_desc);

    // Function Core
    while (1)
    {
        vTaskDelay(100);
    }
}