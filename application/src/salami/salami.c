/**
 * @file    salami.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SALAMI Task
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "salami/salami.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SalamiMain(void)
 * @brief           Main of the SALAMI Task
 */
void SalamiMain(void)
{
    // Initialisation
    deviceNo_t dev_user_led;
    (void)DeviceOpen(&dev_user_led, DEVICE_TYPE_PERIPHERAL, USER_LED, DEVICE_NO_EXTRA_INFO);

    // Function Core
    while (1)
    {
        ConsolePrint("Hello\n");
        (void)DeviceIoctl(dev_user_led, GPIO_IOCTL_TOGGLE, NULL, 0u);

        SleepPeriodic();
    }
}