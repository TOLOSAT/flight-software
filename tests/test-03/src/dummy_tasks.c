/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <stdio.h>
#include <string.h>

#include "dummy_tasks.h"
#include "tasks.h"
#include "fdir.h"
#include "conf/tasks_conf.h"
#include "buffers.h"
#include "conf/buffers_conf.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define OW_MAX_MSG_SIZE     8u      /**< OW max message size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern gpioInst_t led_inst;
extern owInst_t one_wire_inst;

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
    uint8_t ow_msg[OW_MAX_MSG_SIZE] = {0};
    uint8_t temperature = 0u;
    halIoCtlCmd_t ow_init = {OW_IOCTL_INIT_CONNECTION, 0u, NULL};

    // Initialisation
    printf("[#1] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        printf("[#1] Hello\n");
        GpioToggle(&led_inst);

        // Ask for temp conversion
        (void)OwIoctl(&one_wire_inst, ow_init);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0x44u;
        (void)OwWrite(&one_wire_inst, ow_msg, 2u);
        
        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Read temperature
        (void)OwIoctl(&one_wire_inst, ow_init);
        ow_msg[0] = 0xCCu;
        ow_msg[1] = 0xBEu;
        (void)OwWrite(&one_wire_inst, ow_msg, 2u);
        (void)memset(&ow_msg, 0, OW_MAX_MSG_SIZE);
        (void)OwRead(&one_wire_inst, ow_msg, OW_MAX_MSG_SIZE);

        // Update temperature value
        temperature = ow_msg[0] >> 1u;
        printf("[#1] Temperature = %d°C\n", temperature);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}
