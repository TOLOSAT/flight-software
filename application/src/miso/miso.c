/**
 * @file    miso.c
 * @author  Merlin Kooshmanian & Clement Cognard & Yann Awbi
 * @brief   Source file for MISO Task
 * @date    28/06/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "miso/miso.h"
#include "core_basics.h"
#include "tc_execution.h"
#include "miso/pus161.h"

/***************************** Macros Definitions ****************************/

#define MAX_STACK_USAGE     80u     /**< Maximum stack usage authorized in percent */

/*************************** Functions Declarations **************************/

static appStatus_t GetSystemUsage(uint8_t *idle_time, uint8_t *highest_stack_consumer, uint8_t *max_stack_usage);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_miso_execution_table
 * @brief   Execution table for incomming pus 161 TC
 * @warning Keys must be ordered from smallest to largest
 */

pusExecutionTable_t IN_MISO_DATA_SECTION g_miso_execution_table[NB_PUS161_EXECUTION] =
{
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
};

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void *task_dyn_conf)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_dyn_conf Status of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_dyn_conf)
{
    // Variable Initialisation
    uint32_t task_status;
    pus161Data_t pus161_data;

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Initialise PUS161
    task_status = InitS161(&pus161_data);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check system usage
        task_status = GetSystemUsage(&pus161_data.idle_time, &pus161_data.highest_stack_consumer, &pus161_data.max_stack_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS161, TM_PUS161, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate Event
        if (pus161_data.max_stack_usage > MAX_STACK_USAGE)
        {
            // Add more events level (Medium Severity & High Severity ??)
            // Generate event (message -> CARNE -> PUS)
            // Generate message to SALAMI (if high severity event)
            // Generate TM (if not severe only)
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn          GetSystemUsage(uint8_t *idle_time, uint8_t *highest_stack_consumer, uint8_t *max_stack_usage)
 * @brief       Retrieves the system usage.
 * @param[out]  idle_time               idle time in percent
 * @param[out]  highest_stack_consumer  task with the highest stack consumption
 * @param[out]  max_stack_usage         maximum stack usage in percent
 * @retval      #APP_SUCCESSFUL always
 *
 * This function takes a snapshot of the current state of all tasks and calculates
 * the maximum stack usage among them.
 */
static appStatus_t IN_MISO_TEXT_SECTION GetSystemUsage(uint8_t *idle_time, uint8_t *highest_stack_consumer, uint8_t *max_stack_usage)
{
    // Variable Initialisation
    appStatus_t return_value = APP_SUCCESSFUL;
    TaskStatus_t task_status_array[REAL_NB_TASKS] = {0};
    uint8_t highest_stack_consumer_temp = 0u;
    uint8_t max_stack_usage_temp = 0u;

    // First get idle time
    *idle_time = (uint8_t)ulTaskGetIdleRunTimePercent();

    // Take a snapshot of all task states.
    UBaseType_t status_array_size = uxTaskGetSystemState(task_status_array, REAL_NB_TASKS, NULL);

    // Retrieve the task with the highest stack usage.
    for (uint32_t i = 0u; i < status_array_size; i++)
    {
        // Considere only TAPAS tasks (not FreeRTOS hiden ones)
        // WARNING : Check if a reset task change its number
        if ((task_status_array[i].xTaskNumber - 1u) < NB_TASKS)
        {
            uint8_t current_stack_usage = ((g_tasks_static_conf[task_status_array[i].xTaskNumber - 1u].stack_size -
                                           (task_status_array[i].usStackHighWaterMark * sizeof(StackType_t))) * 100u) /
                                           g_tasks_static_conf[task_status_array[i].xTaskNumber - 1u].stack_size;

            if (current_stack_usage > max_stack_usage_temp)
            {
                max_stack_usage_temp = current_stack_usage;
                highest_stack_consumer_temp = task_status_array[i].xTaskNumber - 1u;
            }
        }
    }

    // Update data
    *highest_stack_consumer = highest_stack_consumer_temp;
    *max_stack_usage = max_stack_usage_temp;

    return return_value;
}

// To Do :
// System usage report
// For each task (13)
// - Number (TaskRef_t) ?? (uint8)
// - Stack Usage -> percent (uint8)
// - Time Usage (runtime / uptime ?) -> percent (uint8)
// - Task Mode (in dynamic task table) ?? (uint8)
