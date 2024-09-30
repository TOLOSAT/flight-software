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
 * @fn      DummyMainTask01(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask01(void)
{
    // Variable Initialisation
    deviceNo_t dev_user_led;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    (void)DeviceOpen(&dev_user_led, DEVICE_TYPE_PERIPHERAL, USER_LED, DEVICE_NO_EXTRA_INFO);

    // Function Core
    while (1)
    {
        ConsolePrint("[#1] Hello\n");
        (void)DeviceIoctl(dev_user_led, GPIO_IOCTL_TOGGLE, NULL, 0u);

        SleepPeriodic();
    }
}

/**
 * @fn      DummyMainTask02(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask02(void)
{
    // Initialisation

    // Function Core
    while (1)
    {
        Sleep(100);
    }
}