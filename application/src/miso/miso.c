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

static appStatus_t GetSystemUsage(pus161Data_t *system_usage);

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
    pus161Data_t system_usage = {.number_of_tasks = NB_TASKS};

    // Initialisation
    task_status = InitPeriodicWait(task_dyn_conf);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Initialise PUS161
    task_status = InitS161(&system_usage);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check system usage
        task_status = GetSystemUsage(&system_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS161, TM_PUS161, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate Event
        if (system_usage.max_stack_usage > MAX_STACK_USAGE)
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
 * @fn          GetSystemUsage(void)
 * @brief       Retrieves the system usage.
 * @param[in]   system_usage System Usage as defined in PUS161
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
static appStatus_t IN_MISO_TEXT_SECTION GetSystemUsage(pus161Data_t *system_usage)
{
    // Variable Initialisation
    appStatus_t return_value = APP_SUCCESSFUL;
    TaskStatus_t task_status_array[REAL_NB_TASKS] = {0};
    uint8_t highest_stack_consumer_temp = 0u;
    uint8_t max_stack_usage_temp = 0u;

    // First get idle time
    system_usage->idle_time = (uint8_t)ulTaskGetIdleRunTimePercent();

    // Take a snapshot of all task states.
    UBaseType_t status_array_size = uxTaskGetSystemState(task_status_array, REAL_NB_TASKS, NULL);

    // Retrieve the task with the highest stack usage.
    for (uint32_t i = 0u; i < status_array_size; i++)
    {
        // Get task number from the status array
        uint32_t task = task_status_array[i].xTaskNumber - 1u;

        // Considere only TAPAS tasks (not FreeRTOS hiden ones)
        // WARNING : Check if a reset task change its number
        if (task < NB_TASKS)
        {
            // Get task data
            uint8_t current_stack_usage = ((g_tasks_static_conf[task].stack_size -
                                           (task_status_array[i].usStackHighWaterMark * sizeof(StackType_t))) * 100u) /
                                           g_tasks_static_conf[task].stack_size;

            uint8_t current_time_usage = 0u; // task_status_array[i].ulRunTimeCounter is always zero so for the moment I dont know

            // Update task status in system usage
            system_usage->system_report[task].task_mode = g_tasks_dynamic_conf[task].mode;
            system_usage->system_report[task].stack_usage = current_stack_usage;
            system_usage->system_report[task].time_usage = current_time_usage;

            // Update max usage data if needed
            if (current_stack_usage > max_stack_usage_temp)
            {
                max_stack_usage_temp = current_stack_usage;
                highest_stack_consumer_temp = task_status_array[i].xTaskNumber - 1u;
            }
        }
    }

    // Update max usage data in the system usage
    system_usage->highest_stack_consumer = highest_stack_consumer_temp;
    system_usage->max_stack_usage = max_stack_usage_temp;

    return return_value;
}

