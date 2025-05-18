/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/tasks.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

extern void vInitTaskPrivilege(TaskHandle_t xTask, BaseType_t xRunPrivileged);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that initialises the tasks
 * @return  Nothing
 */
void CreateTasks(void)
{
    taskNo_t task = 1u;

    // Create statically every task
    while (TASK_CONF(task).task != NO_TASK)
    {
        // The stack depth is not in bytes but in words (16 bits, 32 bits, 64 bits
        // depending on the architecture), so stack size need to be a multiple of
        // sizeof(StackType_t)
        if ((TASK_CONF(task).stack_size % sizeof(StackType_t)) == 0u)
        {
            // Create task
            TASK_DESC(task).handle = xTaskCreateStatic(TASK_CONF(task).function,                         // Task function
                                                       TASK_CONF(task).name,                             // Task main
                                                       TASK_CONF(task).stack_size / sizeof(StackType_t), // Task stack size
                                                       NULL,                                             // Task function parameter
                                                       TASK_CONF(task).priority,                         // Task priority
                                                       TASK_CONF(task).p_stack,                          // Task stack
                                                       TASK_CONF(task).p_tcb);                           // Task TCB
            if (TASK_DESC(task).handle == NULL)
            {
                KernelPanic();
            }
            // Set task number in task handle (for easier task recognition)
            vTaskSetTaskNumber(TASK_DESC(task).handle, task);
            // Init task privilege
            vInitTaskPrivilege(TASK_DESC(task).handle, TASK_CONF(task).privilege);
            // Set period
            TASK_DESC(task).period = TASK_CONF(task).default_period;
            task++;
        }
        else
        {
            KernelPanic();
        }
    }
}

/**
 * @fn          GetCurrentTask(void)
 * @brief       Functions that gets the task no of the current task
 * @return      Current task
 *
 * @note If a task is not registered by the TAPAS API, it means either it's a FreeRTOS internal task or badly initialised task
 */
taskNo_t GetCurrentTask(void)
{
    taskNo_t task = NO_TASK;

    // Get current task number
    task = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());
    if (!IS_A_VALID_TASK(task))
    {
        task = NO_TASK;
    }

    return task;
}

/**
 * @fn          SuspendTask(taskNo_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task    Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if halt is successful
 * @retval      #RET_INVALID_PARAM if task ref does not exist
 */
returnCode_t SuspendTask(taskNo_t task)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        // Update task mode for a soft suspension
        TASK_DESC(task).mode = TASK_SUSPENDED;
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
 * @param[in]   task    Reference of the task (in TASKS_ENUM)
 * @retval      #RET_SUCCESSFUL if resume is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 */
returnCode_t ResumeTask(taskNo_t task)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        // Update task mode
        TASK_DESC(task).mode = TASK_NOMINAL;

        // Unlock the task
        vTaskResume(TASK_DESC(task).handle);
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
 * @param[in]   task        Reference of the task (in TASKS_ENUM)
 * @param[out]  priority    Current priority of the task
 * @retval      #RET_SUCCESSFUL if get is successful
 * @retval      #RET_INVALID_PARAM if task does not exist
 */
returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        *priority = uxTaskPriorityGet(TASK_DESC(task).handle);
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
 * @param[in]   task        Reference of the task (in TASKS_ENUM)
 * @param[in]   priority    New priority of the task
 * @retval      #RET_SUCCESSFUL if set is successful
 * @retval      #RET_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_TASK(task))
    {
        vTaskPrioritySet(TASK_DESC(task).handle, priority);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
