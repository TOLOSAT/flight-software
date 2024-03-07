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
    .hw_ctrl_reg = IRIDIUM_ECHO_OFF | IRIDIUM_RING_ALERT_OFF |      // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_VERBOSE_ON | IRIDIUM_QUIET_OFF |         // cppcheck-suppress misra-c2012-12.2; False positive
                   IRIDIUM_HW_CTRL_FLOW_DISABLE | IRIDIUM_DTR_OFF | 
                   IRIDIUM_115200_BPS,
};

/*************************** Functions Definitions ***************************/

/**
 * @fn      DummyTask01(void *task_dyn_conf)
 * @brief   Function that runs the dummy task 01.
 * @param   task_dyn_conf Status of the current task
 */
void DummyTask01(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;

    // Initialisation
    ConsolePrint("[#1] Init\n");
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Toggle LED
        (void)GpioToggle(&led_inst);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn      DummyTask02(void *task_dyn_conf)
 * @brief   Function that runs the dummy task 02.
 * @param   task_dyn_conf Status of the current task
 */
void DummyTask02(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    iridiumNetworkAvailability_t availability = {0};

    // Initialisation
    ConsolePrint("[#2] Init\n");
    (void)IridiumStart(&g_iridium_inst);
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Get Iridium Network
        (void)IridiumGetNetworkAvailability(&g_iridium_inst, &availability);
        ConsolePrint("[#2] Iridium Network availability = ");
        ConsolePrintNumber(availability);
        ConsolePrint("\n");
        (void)(availability);

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}