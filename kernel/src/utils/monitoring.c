/**
 * @file    monitoring.c
 * @author  Merlin Kooshmanian
 * @brief   Monitoring functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/monitoring.h"
#include "core/tasks.h"
#include "drv/drv_tim.h"

/***************************** Macros Definitions ****************************/

#define REAL_NB_TASKS   (NB_TASKS + NB_KERNEL_TASKS) /**< Real number of tasks because kernel internal task are not taken into account in NB_TASKS*/

/*************************** Functions Declarations **************************/

extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_system_usage
 * @brief   System usage struct
 */
systemUsage_t g_system_usage = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitMonitoring(void)
 * @brief   Enables TAPAS monitoring
 * @retval  #RET_ERROR if cannot init timer for monitoring
 * @retval  #RET_SUCCESSFUL else
 */
returnCode_t InitMonitoring(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // First nitialise task ref fields
    for (uint32_t i = 0u; i < NB_TASKS; i++)
    {
        g_system_usage.task_usage[i].task_ref = i + 1u;
    }
    g_system_usage.number_of_tasks = NB_TASKS;

    // Then initialise the timer
    return_value = InitMonitoringTimer();

    return return_value;
}

/**
 * @fn          UpdateSystemUsage(void)
 * @brief       Retrieves the system usage.
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
returnCode_t UpdateSystemUsage(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    TaskStatus_t task_status_array[REAL_NB_TASKS] = {0};
    uint8_t highest_stack_consumer_temp = 0u;
    uint8_t max_stack_usage_temp = 0u;
    uint32_t total_run_time = 0u;

    // First get idle time
    g_system_usage.idle_time = (uint8_t)ulTaskGetIdleRunTimePercent();

    // Take a snapshot of all task states.
    UBaseType_t status_array_size = uxTaskGetSystemState(task_status_array, REAL_NB_TASKS, &total_run_time);

    // Retrieve the task with the highest stack usage.
    for (uint32_t i = 0u; i < status_array_size; i++)
    {
        // Get task number
        // Note : FreeRTOS numbers tasks starting from 1.
        uint32_t task = task_status_array[i].xTaskNumber;

        // Considere only TAPAS tasks (not FreeRTOS internal ones)
        if ((task != 0u) && (task <= (taskNo_t)NB_TASKS))
        {
            // Get task data
            uint8_t current_stack_usage = ((g_tasks_conf[TASKNO_TO_LINENO(task)].stack_size -
                                           (task_status_array[i].usStackHighWaterMark * sizeof(StackType_t))) * 100u) /
                                           g_tasks_conf[TASKNO_TO_LINENO(task)].stack_size;

            uint8_t current_time_usage = (task_status_array[i].ulRunTimeCounter * 100u) / total_run_time;

            // Update task status in system usage
            g_system_usage.task_usage[TASKNO_TO_LINENO(task)].task_mode = g_tasks_desc_table[TASKNO_TO_LINENO(task)].mode;
            g_system_usage.task_usage[TASKNO_TO_LINENO(task)].stack_usage = current_stack_usage;
            g_system_usage.task_usage[TASKNO_TO_LINENO(task)].time_usage = current_time_usage;

            // Update max usage data if needed
            if (current_stack_usage > max_stack_usage_temp)
            {
                max_stack_usage_temp = current_stack_usage;
                highest_stack_consumer_temp = (uint8_t)task;
            }
        }
    }

    // Update max usage data in the system usage
    g_system_usage.highest_stack_consumer = highest_stack_consumer_temp;
    g_system_usage.max_stack_usage = max_stack_usage_temp;

    return return_value;
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