/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 * @date    31/05/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/tasks.h"
#include "utils/monitoring.h"
#include "drv/drv_tim.h"

/***************************** Macros Definitions ****************************/

#define REAL_NB_TASKS   ((uint32_t)NB_TASKS+2u) /**< Real number of tasks (because FreeRTOS adds IdleTask and TimerSVC task) */

/*************************** Functions Declarations **************************/

extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitMonitoring(void)
 * @brief   Enables TAPAS monitoring
 * @retval  #RET_ERROR if cannot init timer for monitoring
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitMonitoring(void)
{
    return InitMonitoringTimer();
}

/**
 * @fn      configureTimerForRunTimeStats(void)
 * @brief   Configures runtime statistics variables
 */
void configureTimerForRunTimeStats(void)
{
    StartMonitoringTimer();
}

/**
 * @fn      getRunTimeCounterValue(void)
 * @brief   Increment runtime counter
 * @return  Current timer tick
 */
unsigned long getRunTimeCounterValue(void)
{
    return GetMonitoringTick();
}

/**
 * @fn          GetSystemUsage(void)
 * @brief       Retrieves the system usage.
 * @param[in]   system_usage    System Usage as defined in PUS161
 * @retval      #APP_SUCCESSFUL always
 *
 * This function will retrieves :
 * - Highest stack consumer
 * - Max stack usage
 * - Idle time
 * And for every task
 * - Stack usage (in percent)
 * - Time usage (in percent)
 * - Task mode (from dynamic task table)
 */
returnCode_t GetSystemUsage(monitoringSystemUsage_t *system_usage)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    TaskStatus_t task_status_array[REAL_NB_TASKS] = {0};
    uint8_t highest_stack_consumer_temp = 0u;
    uint8_t max_stack_usage_temp = 0u;
    uint32_t total_run_time = 0u;

    // First get idle time
    system_usage->idle_time = (uint8_t)ulTaskGetIdleRunTimePercent();

    // Take a snapshot of all task states.
    UBaseType_t status_array_size = uxTaskGetSystemState(task_status_array, REAL_NB_TASKS, &total_run_time);

    // Retrieve the task with the highest stack usage.
    for (uint32_t i = 0u; i < status_array_size; i++)
    {
        // Get task number
        // Note : FreeRTOS numbers tasks starting from 1.
        uint32_t task = task_status_array[i].xTaskNumber - 1u;

        // Considere only TAPAS tasks (not FreeRTOS internal ones)
        if (task < (uint32_t)NB_TASKS)
        {
            // Get task data
            uint8_t current_stack_usage = ((g_tasks_conf[task].stack_size -
                                           (task_status_array[i].usStackHighWaterMark * sizeof(StackType_t))) * 100u) /
                                           g_tasks_conf[task].stack_size;

            uint8_t current_time_usage = (task_status_array[i].ulRunTimeCounter * 100u) / total_run_time;

            // Update task status in system usage
            system_usage->system_report[task].task_mode = g_tasks_desc_table[task].mode;
            system_usage->system_report[task].stack_usage = current_stack_usage;
            system_usage->system_report[task].time_usage = current_time_usage;

            // Update max usage data if needed
            if (current_stack_usage > max_stack_usage_temp)
            {
                max_stack_usage_temp = current_stack_usage;
                highest_stack_consumer_temp = (uint8_t)task;
            }
        }
    }

    // Update max usage data in the system usage
    system_usage->highest_stack_consumer = highest_stack_consumer_temp;
    system_usage->max_stack_usage = max_stack_usage_temp;

    return return_value;
}