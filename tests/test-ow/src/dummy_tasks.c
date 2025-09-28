/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "kernel.h"
#include "conf/system_conf.h"

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
    uint8_t ow_msg[OW_MAX_MSG_SIZE] = { 0 };
    deviceNo_t dev_ow_avionic;
    int16_t raw_temperature = 0;
    float temperature       = 0.0;

    // Initialisation
    (void)DeviceOpen(&dev_ow_avionic, DEVICE_TYPE_PERIPHERAL, ONEWIRE_AVIONIC);

    // Task Core
    while (1)
    {
        LOG("Hello\n");

        // Ask for temp conversion
        (void)DeviceIoctl(dev_ow_avionic, IOCTL_OW_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0x44u;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);

        Sleep(500);

        // Read temperature
        (void)DeviceIoctl(dev_ow_avionic, IOCTL_OW_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0xBEu;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);
        (void)memset(&ow_msg, 0, OW_MAX_MSG_SIZE);
        (void)DeviceRead(dev_ow_avionic, ow_msg, OW_MAX_MSG_SIZE);

        // Update temperature value
        raw_temperature = (ow_msg[1] << 8) | ow_msg[0];
        temperature     = (float)((raw_temperature << 4) >> 4) * 0.0625;
        LOG_DECIMAL("Temperature = %d C\n", (int)temperature);

        SleepPeriodic();
    }
}
