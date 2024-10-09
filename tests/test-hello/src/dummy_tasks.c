/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
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
 * @fn      DummyMainTask(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask(void)
{
    // Variable Initialisation
    rtcTime_t rtc_time;
    deviceNo_t dev_user_led;

    // Initialisation
    ConsolePrint("[#0] Init\n");
    (void)DeviceOpen(&dev_user_led, DEVICE_TYPE_PERIPHERAL, USER_LED, DEVICE_NO_EXTRA_INFO);

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

        SleepPeriodic();
    }
}