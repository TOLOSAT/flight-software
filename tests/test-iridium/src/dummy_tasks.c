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
#include "io_instances.h"
#include "iridium_driver.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

iridiumInst_t g_iridium_inst = 
{
    .uart_inst = &uart_pl_inst,
    .hw_ctrl_reg = IRIDIUM_ECHO_OFF | IRIDIUM_RING_ALERT_OFF | 
                   IRIDIUM_VERBOSE_ON | IRIDIUM_QUIET_OFF | 
                   IRIDIUM_HW_CTRL_FLOW_DISABLE | IRIDIUM_DTR_OFF | 
                   IRIDIUM_115200_BPS,
};

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

    // Initialisation
    ConsolePrint("[#0] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    (void)IridiumStart(&g_iridium_inst);

    // Function Core
    while (1)
    {
        // Toggle LED
        (void)GpioToggle(&led_inst);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}