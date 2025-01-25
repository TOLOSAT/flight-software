/**
 * @file    sysmon.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for system monitoring handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "system/sysmon.h"
#include "core/tasks.h"
#include "drv/drv_tim.h"
#include "drv/drv_wdg.h"
#include "fdir/fdir.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

#define SYSMON_PRIORITY   PRIORITY_EXTREME    /**< SYSMON task priority */
#define SYSMON_STACK_SIZE 2048u               /**< SYSMON task stack size */

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
 * @return  Nothing
 */
void InitMonitoring(void)
{
    // Variable Initialisation
    static taskHandle_t sysmon_task_handle = {0};
    static taskStack_t sysmon_task_stack[SYSMON_STACK_SIZE/sizeof(taskStack_t)] __attribute__((aligned(SYSMON_STACK_SIZE))) = {0};
    static taskTCB_t sysmon_task_tcb = {0};

    // First initialise task ref fields
    for (uint32_t i = 0u; i < NB_TASKS; i++)
    {
        g_system_usage.task_usage[i].task_ref = i + 1u;
    }
    g_system_usage.number_of_tasks = NB_TASKS;

    // Then initialise the timer : if everything went right finally create the SYSMON task
    returnCode_t test_val = InitMonitoringTimer();
    if (test_val == RET_SUCCESSFUL)
    {
        // Function Core
        sysmon_task_handle = xTaskCreateStatic((taskFunction_t)SystemMonitoringMain, "SYSMON",
                                                SYSMON_STACK_SIZE / sizeof(StackType_t),
                                                NULL,SYSMON_PRIORITY, sysmon_task_stack,
                                                &sysmon_task_tcb);
        if (sysmon_task_handle == NULL)
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }
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
        uint32_t task = uxTaskGetTaskNumber(task_status_array[i].xHandle);

        // Considere only TAPAS tasks (not FreeRTOS internal ones)
        if ((task != 0u) && (task <= NB_TASKS))
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
 * @fn              SystemMonitoringMain(void)
 * @brief           Main of the system monitoring updater
 */
void SystemMonitoringMain(void)
{
    // Initialise watchdog
#if defined(CONFIG_WDG)
    CheckError(InitWatchDog(2u * (uint32_t)CONFIG_SYSMON_PERIOD_MS));
#endif

    // Initialisation
    tick_t last_wake = xTaskGetTickCount();

    // Function Core
    while (1)
    {
#if defined(CONFIG_WDG)
        PetWatchDog();
#endif

        // Update the system usage
        CheckError(UpdateSystemUsage());

        // Indicates that the system status is OK
        LOG("System : OK\n");

        // Blink status LED
        LEDStatToggle();

        // Sleep until next period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(CONFIG_SYSMON_PERIOD_MS));
    }
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