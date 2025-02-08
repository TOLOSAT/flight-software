/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "kernel.h"

/***************************** Macros Definitions ****************************/

#define OW_MAX_MSG_SIZE 8u /**< OW max message size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyMainTask(void)
 * @brief   Function that runs the dummy main task.
 */
void DummyMainTask(void)
{
    // Initialisation
    uint8_t ow_msg[OW_MAX_MSG_SIZE] = { 0 };
    deviceNo_t dev_ow_avionic;
    (void)DeviceOpen(&dev_ow_avionic, DEVICE_TYPE_PERIPHERAL, ONEWIRE_AVIONIC);

    // Function Core
    while (1)
    {
        uint8_t temperature = 0u;
        LOG("Hello\n");

        // Ask for temp conversion
        (void)DeviceIoctl(dev_ow_avionic, IOCTL_OW_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0x44u;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);

        SleepPeriodic();

        // Read temperature
        (void)DeviceIoctl(dev_ow_avionic, IOCTL_OW_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0xBEu;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);
        (void)memset(&ow_msg, 0, OW_MAX_MSG_SIZE);
        (void)DeviceRead(dev_ow_avionic, ow_msg, OW_MAX_MSG_SIZE);

        // Update temperature value
        temperature = ow_msg[0] >> 1u;
        LOG_DECIMAL("Temperature = %d C\n", temperature);

        SleepPeriodic();
    }
}
