/**
 * @file    dummy_tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file with dummy tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "dummy_tasks.h"
#include "kernel.h"
#include "bsp.h"
#include "system_conf.h"

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
    deviceNo_t dev_system_usage = 0u;
    systemUsage_t system_usage  = { 0 };

    // Initialisation
    LOG("Init dummy task\n");
    (void)DeviceOpen(&dev_system_usage, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_USAGE);

    // Task Core
    while (1)
    {
        // Print current usage
        (void)DeviceRead(dev_system_usage, (data_t)&system_usage, sizeof(systemUsage_t));
        LOG_DECIMAL("Main task usage : %d%\n", system_usage.task_usage[0].time_usage);

        // Consume 75% CPU (active wait)
        HAL_Delay(375);

        // Wait for next period
        SleepPeriodic();

        // Print current usage
        (void)DeviceRead(dev_system_usage, (data_t)&system_usage, sizeof(systemUsage_t));
        LOG_DECIMAL("Main task usage : %d%\n", system_usage.task_usage[0].time_usage);

        // Consume 25% CPU (active wait)
        HAL_Delay(125);

        // Wait for next period
        SleepPeriodic();
    }
}