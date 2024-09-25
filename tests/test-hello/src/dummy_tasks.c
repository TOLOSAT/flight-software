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
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void *task_desc)
 * @brief   Function that runs the dummy main task.
 * @param   task_desc Descriptor of the current task
 */
void DummyMainTask(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    rtcTime_t rtc_time;
    deviceNo_t dev_user_led;

    // Initialisation
    ConsolePrint("[#0] Init\n");
    task_status = DeviceOpen(&dev_user_led, USER_LED, 0u);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Get time
        (void)RtcGetTime(&rtc_time);
        ConsolePrint("[#0] Current date : ");
        ConsolePrintNumber(rtc_time.day);
        ConsolePrint("-");
        ConsolePrintNumber(rtc_time.month);
        ConsolePrint("-");
        ConsolePrintNumber(rtc_time.year);
        ConsolePrint(" ");
        ConsolePrintNumber(rtc_time.hour);
        ConsolePrint(":");
        ConsolePrintNumber(rtc_time.minute);
        ConsolePrint(":");
        ConsolePrintNumber(rtc_time.second);
        ConsolePrint("\n");

        // Toggle LED
        (void)DeviceIoctl(dev_user_led, GPIO_IOCTL_TOGGLE, NULL, 0u);

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}