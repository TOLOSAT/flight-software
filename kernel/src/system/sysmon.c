/**
 * @file    sysmon.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for system monitoring handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/sysmon.h"
#include "core/tasks.h"
#include "drv/others/drv_tim.h"
#include "fdir/fdir.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

#define SYSMON_PERIOD_MS    500u         /**< SYSMON task period */
#define SYSMON_PRIORITY     PRIORITY_LOW /**< SYSMON task priority */
#define SYSMON_STACK_SIZE   2048u        /**< SYSMON task stack size */

/**
 * @def     TASK_USAGE(task_no)
 * @brief   Get task usage from g_system_usage
 */
#define TASK_USAGE(task_no) (g_system_usage.task_usage[(task_no) - 1u])

/*************************** Functions Declarations **************************/

static returnCode_t UpdateSystemUsage(void);
extern void configureTimerForRunTimeStats(void);
extern unsigned long getRunTimeCounterValue(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_system_usage
 * @brief   System usage struct
 */
systemUsage_t g_system_usage = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitSYSMON(void)
 * @brief   Enables System Monitoring
 * @return  Nothing
 */
void InitSYSMON(void)
{
    static taskHandle_t sysmon_task_handle                                                                                    = { 0 };
    static taskStack_t sysmon_task_stack[SYSMON_STACK_SIZE / sizeof(taskStack_t)] __attribute__((aligned(SYSMON_STACK_SIZE))) = { 0 };
    static taskTCB_t sysmon_task_tcb                                                                                          = { 0 };

    // First initialise task ref fields
    taskNo_t task = 1u;
    while (TASK_CONF(task).task != NO_TASK)
    {
        TASK_USAGE(task).task_ref = (uint8_t)task;
        task++;
    }
    g_system_usage.number_of_tasks = (uint8_t)task - 1u;

    // Then initialise the timer : if everything went right finally create the SYSMON task
    returnCode_t test_val = InitMonitoringTimer();
    if (test_val == RET_SUCCESSFUL)
    {
        // Then create Sysmon Task
        sysmon_task_handle = xTaskCreateStatic((taskFunction_t)SYSMONMain, "SYSMON", SYSMON_STACK_SIZE / sizeof(StackType_t), NULL, SYSMON_PRIORITY,
                                               sysmon_task_stack, &sysmon_task_tcb);
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
 * @fn              SYSMONMain(void)
 * @brief           Main of the SYSMON task
 */
void SYSMONMain(void)
{
    // Initialisation
    tick_t last_wake = xTaskGetTickCount();

    // Task Core
    while (1)
    {
        // Update the system usage
        CheckError(UpdateSystemUsage());

        // Indicates that the system status is OK
        LOG("System : OK\n");

        // Blink status LED
        LEDStatToggle();

        // Sleep until next period
        vTaskDelayUntil(&last_wake, pdMS_TO_TICKS(SYSMON_PERIOD_MS));
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
static returnCode_t UpdateSystemUsage(void)
{
    returnCode_t return_value          = RET_SUCCESSFUL;
    taskNo_t task                      = 1u;
    uint8_t tmp_highest_stack_consumer = 0u;
    uint8_t tmp_max_stack_usage        = 0u;
    uint32_t tmp_current_runtime_counter;
    uint32_t tmp_current_total_counter;

    // Old counter
    static uint32_t last_idle_runtime_counter                      = 0u;
    static uint32_t last_idle_total_counter                        = 0u;
    static uint32_t last_task_runtime_counter[CONFIG_MAX_NB_TASKS] = { 0 };
    static uint32_t last_task_total_counter[CONFIG_MAX_NB_TASKS]   = { 0 };

    // First get idle time
    tmp_current_runtime_counter = ulTaskGetIdleRunTimeCounter();
    tmp_current_total_counter   = getRunTimeCounterValue();
    g_system_usage.idle_time =
        (uint8_t)(100u * (tmp_current_runtime_counter - last_idle_runtime_counter) / (tmp_current_total_counter - last_idle_total_counter));
    last_idle_runtime_counter = tmp_current_runtime_counter;
    last_idle_total_counter   = tmp_current_total_counter;

    // Retrieve statistics for every task
    while (IS_A_VALID_TASK(task))
    {
        TaskStatus_t task_status = { 0 };

        // Get task statistics
        vTaskGetInfo(TASK_DESC(task).handle, &task_status, pdTRUE, eInvalid);

        // Get task stack usage
        TASK_USAGE(task).stack_usage =
            ((TASK_CONF(task).stack_size - (task_status.usStackHighWaterMark * sizeof(StackType_t))) * 100u) / TASK_CONF(task).stack_size;

        // Get task relative cpu usage
        tmp_current_runtime_counter     = task_status.ulRunTimeCounter;
        tmp_current_total_counter       = getRunTimeCounterValue();
        TASK_USAGE(task).time_usage     = (uint8_t)(100u * (tmp_current_runtime_counter - last_task_runtime_counter[task])
                                                / (tmp_current_total_counter - last_task_total_counter[task]));
        last_task_runtime_counter[task] = tmp_current_runtime_counter;
        last_task_total_counter[task]   = tmp_current_total_counter;

        // Get task mode
        TASK_USAGE(task).task_mode = TASK_DESC(task).mode;

        // Update max usage data if needed
        if (TASK_USAGE(task).stack_usage > tmp_max_stack_usage)
        {
            tmp_max_stack_usage        = TASK_USAGE(task).stack_usage;
            tmp_highest_stack_consumer = (uint8_t)task;
        }

        // Switch to the next task
        task++;
    }

    // Update max usage data in the system usage
    g_system_usage.highest_stack_consumer = tmp_highest_stack_consumer;
    g_system_usage.max_stack_usage        = tmp_max_stack_usage;

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