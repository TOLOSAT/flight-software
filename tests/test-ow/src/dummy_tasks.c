/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "dummy_tasks.h"
#include "core.h"

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

    // Initialisation
    ConsolePrint("[#1] Init\n");
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        uint8_t temperature = 0u;
        ConsolePrint("[#1] Hello\n");
        (void)GpioToggle(&g_user_led_inst);

        // Ask for temp conversion
        (void)OwIoctl(&g_onewire_avionic_inst, OW_IOCTL_INIT_CONNECTION, NULL, 0u);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0x44u;
        (void)OwWrite(&g_onewire_avionic_inst, ow_msg, 2u);
        
        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Read temperature
        (void)OwIoctl(&g_onewire_avionic_inst, ow_init);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0xBEu;
        (void)OwWrite(&g_onewire_avionic_inst, ow_msg, 2u);
        (void)memset(&ow_msg, 0, OW_MAX_MSG_SIZE);
        (void)OwRead(&g_onewire_avionic_inst, ow_msg, OW_MAX_MSG_SIZE);

        // Update temperature value
        temperature = ow_msg[0] >> 1u;
        ConsolePrint("[#1] Temperature = ");
        ConsolePrintNumber(temperature);
        ConsolePrint(" C\n");

        task_status = WaitUntilNextPeriod(task_desc);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
