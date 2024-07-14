/**
 * @file    miso.c
 * @author  Merlin Kooshmanian & Clement Cognard
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
#define MIN_IDLE_TIME       10u     /**< Minimum idle time authorized in percent */

/*************************** Functions Declarations **************************/

static appStatus_t GetIdleTime(uint32_t *idle_time);
static appStatus_t GetMaxStackUsage(uint32_t *highest_stack_consumer, uint32_t *max_stack_usage);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_miso_execution_table
 * @brief   Execution table for incomming pus 161 TC
 * @warning Keys must be ordered from smallest to largest
 */

pusExecutionTable_t g_miso_execution_table[NB_PUS161_EXECUTION] =
{
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 1u), ExecuteS161SS1, TM_REQUESTED},
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 3u), ExecuteS161SS3, TM_REQUESTED},
    {BUILD_ROUTING_KEY(OBC_APID, 161u, 5u), ExecuteS161SS5, TM_REQUESTED},
};

/**
 * @var     systemState
 * @brief   System state
 */
char systemState[100];

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
        // Check stack usage
        task_status = GetMaxStackUsage(&pus161_data.highest_stack_consumer, &pus161_data.max_stack_usage);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Check runtimes
        task_status = GetIdleTime(&pus161_data.idle_time);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);

        // Executes a TC.
        const tcExecutionBasicBuffers_t basic_buffers = {TC_PUS161, TM_PUS161, TM_PUS1};
        task_status = ExecuteTC((pusExecutionTable_t *)&g_miso_execution_table, NB_PUS161_EXECUTION, basic_buffers);
        CheckErrors(task_status, FDIR_NO_SANCTION);

        // Generate report
        if ((pus161_data.max_stack_usage > MAX_STACK_USAGE) || (pus161_data.idle_time < MIN_IDLE_TIME))
        {
            // TODO : Generate report
        }

        task_status = WaitUntilNextPeriod(task_dyn_conf);
        CheckErrors(task_status, FDIR_ERROR_HANDLER);
    }
}

/**
 * @fn          GetIdleTime(uint32_t *idle_time)
 * @brief       Retrieves the idle time of the system.
 * @param[out]  idle_time Idle time in percent
 * @retval      #APP_SUCCESSFUL always
 *
 * This function returns the percentage of time that the system has been idle.
 */
static appStatus_t GetIdleTime(uint32_t *idle_time)
{
    // Variable Initialisation
    appStatus_t return_value = APP_SUCCESSFUL;
     
    // Function Core
    *idle_time = (uint32_t)ulTaskGetIdleRunTimePercent();

    return return_value;
}

/**
 * @fn          GetMaxStackUsage(uint32_t *highest_stack_consumer, uint32_t *max_stack_usage)
 * @brief       Retrieves the maximum stack usage among all tasks in the system.
 * @param[out]  highest_stack_consumer task with the highest stack consumption
 * @param[out]  max_stack_usage maximum stack usage in percent
 * @retval      #APP_SUCCESSFUL else
 *
 * This function takes a snapshot of the current state of all tasks and calculates
 * the maximum stack usage among them.
 */
static appStatus_t GetMaxStackUsage(uint32_t *highest_stack_consumer, uint32_t *max_stack_usage)
{
    // Variable Initialisation
    appStatus_t return_value = APP_SUCCESSFUL;
    TaskStatus_t task_status_array[REAL_NB_TASKS] = {0};
    uint32_t highest_stack_consumer_temp = 0u;
    uint32_t max_stack_usage_temp = 0u;

    // Take a snapshot of all task states.
    UBaseType_t status_array_size = uxTaskGetSystemState(task_status_array, REAL_NB_TASKS, NULL);

    // Check if there is no problem with the array
    if (status_array_size == REAL_NB_TASKS)
    {
        // Retrieve the task with the highest stack usage.
        for (uint32_t i = 0u; i < REAL_NB_TASKS; i++)
        {
            // Considere only TAPAS tasks (not FreeRTOS hiden ones)
            if ((task_status_array[i].xTaskNumber - 1u) < NB_TASKS)
            {
                uint32_t current_stack_usage = ((g_tasks_static_conf[task_status_array[i].xTaskNumber - 1u].stack_size -
                                                (task_status_array[i].usStackHighWaterMark*sizeof(StackType_t))) * 100) /
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
    }
    else
    {
        return_value = APP_ERROR;
    }

    return return_value;
}

// Need to be written properly
// /**
//  * @fn getState(void)
//  * @brief Returns the system state as a string.
//  * @return A pointer to a string representing the system state.
//  *
//  * The system state includes information about each task in the system, such as task name, current state,
//  * stack high watermark, task number, base priority, current priority, and run time counter.
//  */
// static char* getState(void)
// {
//     TaskStatus_t task_status_array[NB_TASKS*2];
//     uxTaskGetSystemState( task_status_array, NB_TASKS*2, NULL );
//     for (int i = 0; i < NB_TASKS; i++)
//     {
//         sprintf(systemState, "%s,%d,%d,%lu,%lu,%lu,%lu", task_status_array[i].pcTaskName,
//                                                         task_status_array[i].eCurrentState,
//                                                         task_status_array[i].usStackHighWaterMark,
//                                                         (unsigned long)task_status_array[i].xTaskNumber,
//                                                         (unsigned long)task_status_array[i].uxBasePriority,
//                                                         (unsigned long)task_status_array[i].uxCurrentPriority,
//                                                         (unsigned long)task_status_array[i].ulRunTimeCounter);
//     }
//     return systemState;
// }
