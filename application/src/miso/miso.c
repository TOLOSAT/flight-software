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
#include "core.h"
#include "pus.h"
#include "services/pus161.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS161_EXECUTION     3u                       /**< Number of pus161 exution functions */
#define REAL_NB_TASKS           ((uint32_t)NB_TASKS+2u) /**< Real number of tasks (because FreeRTOS adds IdleTask and TimerSVC task) */
#define MAX_STACK_USAGE         80u                     /**< Maximum stack usage authorized in percent */

/*************************** Functions Declarations **************************/

static appStatus_t GetSystemUsage(pus161Data_t *system_usage);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              MisoMain(void *task_desc)
 * @brief           Main of the MISO Task
 * @param[in,out]   task_desc Descriptor of the current task
 */
void IN_MISO_TEXT_SECTION MisoMain(void *task_desc)
{
    // Variable Initialisation
    uint32_t task_status;
    static pusExecutionTable_t IN_MISO_DATA_SECTION miso_execution_table[NB_PUS161_EXECUTION] =
    {
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
        {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
    };
    pus161Data_t *system_usage = NULL;

    // Initialisation
    task_status = InitPeriodicWait(task_desc);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);
    task_status = CheckExecutionTable((pusExecutionTable_t *)&miso_execution_table, NB_PUS161_EXECUTION);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Initialise PUS161
    task_status = InitS161(NB_TASKS, &system_usage);
    CheckErrors(task_status, FDIR_ERROR_HANDLER);

    // Function Core
    while (1)
    {
        // Check system usage
        task_status = GetSystemUsage(system_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        task_status = ExecuteTC((pusExecutionTable_t *)&miso_execution_table, NB_PUS161_EXECUTION, TC_PUS161, TM_PUS161, TM_PUS1);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate Event
        if (system_usage->max_stack_usage > MAX_STACK_USAGE)
        {
            // Add more events level (Medium Severity & High Severity ??)
            // Generate event (message -> CARNE -> PUS)
            // Generate message to SALAMI (if high severity event)
            // Generate TM (if not severe only)
        }

        task_status = WaitUntilNextPeriod(task_desc);
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

