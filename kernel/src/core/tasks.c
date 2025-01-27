/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 *
 * @copyright Copyright (c) TOLOSAT 2024
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
    // Variable Initialisation
    taskNo_t task = 1u;

    // Function Core
    while (task <= NB_TASKS)
    {
        // The stack depth is not in bytes but in words (16 bits, 32 bits, 64 bits
        // depending on the architecture), so stack size need to be a multiple of
        // sizeof(StackType_t)
        if ((g_tasks_conf[TASKNO_TO_LINENO(task)].stack_size % sizeof(StackType_t)) == 0u)
        {
            // Create task
            g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle = xTaskCreateStatic(g_tasks_conf[TASKNO_TO_LINENO(task)].function,
                                                                                  g_tasks_conf[TASKNO_TO_LINENO(task)].name,
                                                                                  g_tasks_conf[TASKNO_TO_LINENO(task)].stack_size / sizeof(StackType_t),
                                                                                  NULL,
                                                                                  g_tasks_conf[TASKNO_TO_LINENO(task)].priority,
                                                                                  g_tasks_conf[TASKNO_TO_LINENO(task)].p_stack,
                                                                                  g_tasks_conf[TASKNO_TO_LINENO(task)].p_tcb);
            if (g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle == NULL)
            {
                KernelPanic();
            }
            // Set task number in task handle (for easier task recognition)
            vTaskSetTaskNumber(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle, task);
            // Init task privilege
            vInitTaskPrivilege(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle, g_tasks_conf[TASKNO_TO_LINENO(task)].privilege);
            // Set period
            g_tasks_desc_table[TASKNO_TO_LINENO(task)].period = g_tasks_conf[TASKNO_TO_LINENO(task)].default_period;
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
    // Variable Initialisation
    taskNo_t task = NO_TASK;

    // Function Core
    task = uxTaskGetTaskNumber(xTaskGetCurrentTaskHandle());
    if (task > NB_TASKS)
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((task != NO_TASK) && (task <= NB_TASKS))
    {
        // Update task mode for a soft suspension
        g_tasks_desc_table[TASKNO_TO_LINENO(task)].mode = TASK_SUSPENDED;
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((task != NO_TASK) && (task <= NB_TASKS))
    {
        // Update task mode
        g_tasks_desc_table[TASKNO_TO_LINENO(task)].mode = TASK_NOMINAL;

        // Unlock the task
        vTaskResume(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle);
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((task != NO_TASK) && (task <= NB_TASKS))
    {
        *priority = uxTaskPriorityGet(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle);
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
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((task != NO_TASK) && (task <= NB_TASKS))
    {
        vTaskPrioritySet(g_tasks_desc_table[TASKNO_TO_LINENO(task)].handle, priority);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
