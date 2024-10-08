/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 * @date    21/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/tasks.h"

/***************************** Macros Definitions ****************************/

#define TASK_NB_HANDLE_OFFSET 1u  /**< Offset between the internal task nb of FreeRTOS and taskNo_t in order to have 0 as a non registered task */

/*************************** Functions Declarations **************************/

static returnCode_t GetCurrentTask(taskNo_t *task);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that initialises the tasks
 * @retval  #RET_SUCCESSFUL if creation succeed
 * @retval  #RET_INVALID_PARAM if stack size is not a multiple of sizeof(StackType_t)
 * @retval  #RET_ERROR if at least one task creation failed
 */
returnCode_t CreateTasks(void)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    taskNo_t task = 0u;

    // Function Core
    while ((task < (taskNo_t)NB_TASKS) && (return_value == RET_SUCCESSFUL))
    {
        // The stack depth is not in bytes but in words (16 bits, 32 bits, 64 bits 
        // depending on the architecture), so stack size need to be a multiple of
        // sizeof(StackType_t)
        if ((g_tasks_conf[task].stack_size % sizeof(StackType_t)) == 0u)
        {
            // Create task
            g_tasks_desc_table[task].handle = xTaskCreateStatic(g_tasks_conf[task].function,
                                                                g_tasks_conf[task].name,
                                                                g_tasks_conf[task].stack_size / sizeof(StackType_t),
                                                                NULL,
                                                                g_tasks_conf[task].priority,
                                                                g_tasks_conf[task].p_stack,
                                                                g_tasks_conf[task].p_tcb);
            if (g_tasks_desc_table[task].handle == NULL)
            {
                return_value = RET_ERROR;
            }
            // Set task number in task handle (for easier task recognition)
            vTaskSetTaskNumber(g_tasks_desc_table[task].handle, task + TASK_NB_HANDLE_OFFSET);
            // Set period
            g_tasks_desc_table[task].period = g_tasks_conf[task].default_period;
            task++;
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          SuspendTask(taskNo_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if halt is successful
 * @retval      #RET_ERROR if cannot release task's mutexes
 * @retval      #RET_INVALID_PARAM if task ref does not exist
 */
returnCode_t SuspendTask(taskNo_t task)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        // Update task mode for a soft suspension
        g_tasks_desc_table[task].mode = TASK_SUSPENDED;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ResumeTask(taskNo_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if resume is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 */
returnCode_t ResumeTask(taskNo_t task)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        // Update task mode
        g_tasks_desc_table[task].mode = TASK_NOMINAL;

        // Unlock the task
        vTaskResume(g_tasks_desc_table[task].handle);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[in]   priority New priority of the task
 * @retval      #RET_SUCCESSFUL if set is successful
 * @retval      #RET_ERROR if set cannot be performed
 * @retval      #RET_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        vTaskPrioritySet(g_tasks_desc_table[task].handle, priority);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[out]  priority Current priority of the task
 * @retval      #RET_SUCCESSFUL if get is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 * @retval      #RET_ERROR if get cannot be performed
 */
returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        *priority = uxTaskPriorityGet(g_tasks_desc_table[task].handle);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}


/**
 * @fn          Sleep(uint32_t tick)
 * @brief       Function that puts to sleep the current task.
 * @param[in]   tick Amount of time the task will be put to sleep.
 * @return      Nothing
 * 
 * @note Using tick = 0 will make the task yielding instead.
 */
void Sleep(uint32_t tick)
{
    // Variable Initialisation
    taskNo_t current_task;

    // First gets current task no
    if (GetCurrentTask(&current_task) == RET_SUCCESSFUL)
    {
        // Check First if a suspension is require or not
        if (g_tasks_desc_table[current_task].mode == TASK_SUSPENDED)
        {
            // Suspend the task
            vTaskSuspend(g_tasks_desc_table[current_task].handle);
        }
        else
        {
            // Yielding instead of sleeping when tick equal to zero
            if (tick == 0u)
            {
                taskYIELD();
            }
            else
            {
                vTaskDelay(tick);
            }

            // Check if task has not been suspended during the sleep
            if (g_tasks_desc_table[current_task].mode == TASK_SUSPENDED)
            {
                // Suspend the task
                vTaskSuspend(g_tasks_desc_table[current_task].handle);
            }
        }
    }
}

/**
 * @fn      SleepPeriodic(void)
 * @brief   Function that puts to sleep the current task until next period
 * @return  Nothing
 */
void SleepPeriodic(void)
{
    // Variable Initialisation
    taskNo_t current_task;

    // First gets current task no
    if (GetCurrentTask(&current_task) == RET_SUCCESSFUL)
    {
        // Check First if a suspension is require or not
        if (g_tasks_desc_table[current_task].mode == TASK_SUSPENDED)
        {
            // Suspend the task
            vTaskSuspend(g_tasks_desc_table[current_task].handle);
        }
        else
        {
            // Before sleeping check if we missed period
            if (xTaskGetTickCount() <= (g_tasks_desc_table[current_task].last_wake + g_tasks_desc_table[current_task].period))
            {
                // If period not missed, wait until next period
                xTaskDelayUntil(&g_tasks_desc_table[current_task].last_wake, g_tasks_desc_table[current_task].period);
            }
            else
            {
                // Yield instead
                taskYIELD();
            }

            // Check if task has not been suspended during the sleep
            if (g_tasks_desc_table[current_task].mode == TASK_SUSPENDED)
            {
                // Suspend the task
                vTaskSuspend(g_tasks_desc_table[current_task].handle);
            }
        }

        // Update last wake time anyway
        g_tasks_desc_table[current_task].last_wake = xTaskGetTickCount();
    }
}

/**
 * @fn          GetCurrentTask(taskNo_t *task)
 * @brief       Functions that gets the task no of the current task
 * @param[out]  task 
 * @retval      #RET_ERROR if current task is not registered by the TAPAS API
 * @retval      #RET_SUCCESSFUL else
 * 
 * @note If a task is not registered by the TAPAS API, it means either it's a FreeRTOS internal task or badly initialised task
 */
static returnCode_t GetCurrentTask(taskNo_t *task)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    taskNo_t temp_task_no = 0u;

    // Function Core
    temp_task_no = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());
    if (temp_task_no != 0u)
    {
        *task = temp_task_no - TASK_NB_HANDLE_OFFSET;
    }
    else
    {
        return_value = RET_ERROR;
    }

    return return_value;
}
