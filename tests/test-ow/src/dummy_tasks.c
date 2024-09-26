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

#define OW_MAX_MSG_SIZE     8u      /**< OW max message size */

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
    uint8_t ow_msg[OW_MAX_MSG_SIZE] = {0};
    deviceNo_t dev_user_led;
    deviceNo_t dev_ow_avionic;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    task_status = DeviceOpen(&dev_user_led, DEVICE_TYPE_PERIPHERAL, USER_LED, DEVICE_NO_EXTRA_DATA);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = DeviceOpen(&dev_ow_avionic, DEVICE_TYPE_PERIPHERAL, ONEWIRE_AVIONIC, DEVICE_NO_EXTRA_DATA);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        uint8_t temperature = 0u;
        ConsolePrint("[#1] Hello\n");
        (void)DeviceIoctl(dev_user_led, GPIO_IOCTL_TOGGLE, NULL, 0u);

        // Ask for temp conversion
        (void)DeviceIoctl(dev_ow_avionic, OW_IOCTL_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0x44u;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);
        
        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Read temperature
        (void)DeviceIoctl(dev_ow_avionic, OW_IOCTL_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0xBEu;
        (void)DeviceWrite(dev_ow_avionic, ow_msg, 2u);
        (void)memset(&ow_msg, 0, OW_MAX_MSG_SIZE);
        (void)DeviceRead(dev_ow_avionic, ow_msg, OW_MAX_MSG_SIZE);

        // Update temperature value
        temperature = ow_msg[0] >> 1u;
        ConsolePrint("[#1] Temperature = ");
        ConsolePrintNumber(temperature);
        ConsolePrint(" C\n");

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
