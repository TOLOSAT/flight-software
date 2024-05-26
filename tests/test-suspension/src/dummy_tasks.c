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
#include "platform.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask01(void *task_dyn_conf)
 * @brief   Function that runs the dummy main task.
 * @param   task_dyn_conf Status of the current task
 */
void DummyMainTask01(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    coreStatus_t test;
    int task2_killed = 0;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        (void)GpioToggle(&led_inst);

        test = AcquireMutex(DUMMY_MUTEX);
        if (test == CORE_SUCCESSFUL)
        {
            ConsolePrint("[#1] Mutex Acquired\n");

            task_status = WaitUntilNextPeriod(task_dyn_conf);
            CheckErrors(task_status, FDIR_ERROR_HANDLER);

            test = ReleaseMutex(DUMMY_MUTEX);
            if (test == CORE_SUCCESSFUL)
            {
                ConsolePrint("[#1] Mutex Released\n");
            }
            else
            {
                ConsolePrint("[#1] Error releasing the mutex\n");
            }
        }
        else
        {
            ConsolePrint("[#1] Error acquiring the mutex\n");
        }

        if (task2_killed == 0)
        {
            SuspendTask(SECOND_TASK);
            task2_killed = 1;
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn      DummyMainTask02(void *task_dyn_conf)
 * @brief   Function that runs the dummy main task.
 * @param   task_dyn_conf Status of the current task
 */
void DummyMainTask02(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    coreStatus_t test;

    // Initialisation
    ConsolePrint("[#2] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        test = AcquireMutex(DUMMY_MUTEX);
        if (test == CORE_SUCCESSFUL)
        {
            ConsolePrint("[#2] Mutex Acquired\n");

            task_status = WaitUntilNextPeriod(task_dyn_conf);
            CheckErrors(task_status, FDIR_ERROR_HANDLER);

            test = ReleaseMutex(DUMMY_MUTEX);
            if (test == CORE_SUCCESSFUL)
            {
                ConsolePrint("[#2] Mutex Released\n");
            }
            else
            {
                ConsolePrint("[#2] Error releasing the mutex\n");
            }
        }
        else
        {
            ConsolePrint("[#2] Error acquiring the mutex\n");
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}