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
    ConsolePrint("[#0] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Get time
        (void)RtcGetTime(&rtc_time);
        ConsolePrint("[#0] Time since boot :\n");
        ConsolePrintNumber(rtc_time.hour);
        ConsolePrint(" hour ");
        ConsolePrintNumber(rtc_time.minute);
        ConsolePrint(" min ");
        ConsolePrintNumber(rtc_time.second);
        ConsolePrint(" sec ");
        ConsolePrintNumber(rtc_time.millisecond);
        ConsolePrint(" ms\n");

        // Toggle LED
        (void)GpioToggle(&led_inst);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}