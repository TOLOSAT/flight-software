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
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that initialises the tasks
 * @retval  #KERNEL_SUCCESSFUL if creation succeed
 * @retval  #KERNEL_INVALID_PARAM if stack size is not a multiple of sizeof(StackType_t)
 * @retval  #KERNEL_ERROR if at least one task creation failed
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION CreateTasks(void)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    taskNo_t task = 0u;

    // Function Core
    while ((task < (taskNo_t)NB_TASKS) && (return_value == KERNEL_SUCCESSFUL))
    {
        // The stack depth is not in bytes but in words (16 bits, 32 bits, 64 bits 
        // depending on the architecture), so stack size need to be a multiple of
        // sizeof(StackType_t)
        if ((g_tasks_conf[task].stack_size % sizeof(StackType_t)) == 0u)
        {
#if defined(CONFIG_MPU)
            BaseType_t test_value = pdPASS;
            TaskParameters_t task_parameters =
                {
                    .pvTaskCode = g_tasks_conf[task].function,
                    .pcName = g_tasks_conf[task].name,
                    .usStackDepth = g_tasks_conf[task].stack_size / sizeof(StackType_t),
                    .pvParameters = &g_tasks_desc_table[task],
                    .uxPriority = g_tasks_conf[task].priority,
                    .puxStackBuffer = g_tasks_conf[task].p_stack,
                    .pxTaskBuffer = g_tasks_conf[task].p_tcb,
                };
            // Add Privileged bit if task is privileged
            if (g_tasks_conf[task].privilege == TASK_PRIVILEGED)
            {
                task_parameters.uxPriority |= portPRIVILEGE_BIT;
            }
            // Create task
            test_value = xTaskCreateRestrictedStatic(&task_parameters, &g_tasks_desc_table[task].handle);
            if (test_value != pdPASS)
            {
                return_value = KERNEL_ERROR;
            }
#else
            // Create task
            g_tasks_desc_table[task].handle = xTaskCreateStatic(g_tasks_conf[task].function,
                                                                g_tasks_conf[task].name,
                                                                g_tasks_conf[task].stack_size / sizeof(StackType_t),
                                                                &g_tasks_desc_table[task],
                                                                g_tasks_conf[task].priority,
                                                                g_tasks_conf[task].p_stack,
                                                                g_tasks_conf[task].p_tcb);
            if (g_tasks_desc_table[task].handle == NULL)
            {
                return_value = KERNEL_ERROR;
            }
#endif
            g_tasks_desc_table[task].period = g_tasks_conf[task].default_period;
            task++;
        }
        else
        {
            return_value = KERNEL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          SuspendTask(taskNo_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #KERNEL_SUCCESSFUL if halt is successful
 * @retval      #KERNEL_ERROR if cannot release task's mutexes
 * @retval      #KERNEL_INVALID_PARAM if task ref does not exist
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION SuspendTask(taskNo_t task)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        // Update task mode for a soft suspension
        g_tasks_desc_table[task].mode = TASK_SUSPENDED;
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ResumeTask(taskNo_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #KERNEL_SUCCESSFUL if resume is successful
 * @retval      #KERNEL_INVALID_PARAM if task does not exist
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION ResumeTask(taskNo_t task)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

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
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetTaskPriority(taskNo_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[in]   priority New priority of the task
 * @retval      #KERNEL_SUCCESSFUL if set is successful
 * @retval      #KERNEL_ERROR if set cannot be performed
 * @retval      #KERNEL_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION SetTaskPriority(taskNo_t task, taskPriority_t priority)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        vTaskPrioritySet(g_tasks_desc_table[task].handle, priority);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetTaskPriority(taskNo_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[out]  priority Current priority of the task
 * @retval      #KERNEL_SUCCESSFUL if get is successful
 * @retval      #KERNEL_INVALID_PARAM if task does not exist
 * @retval      #KERNEL_ERROR if get cannot be performed
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION GetTaskPriority(taskNo_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (task < (taskNo_t)NB_TASKS)
    {
        *priority = uxTaskPriorityGet(g_tasks_desc_table[task].handle);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InitPeriodicWait(taskDesc_t *task_desc)
 * @brief       Function that init the last_wake variable in status
 * @param[in]   task_desc Pointer to the task descriptor of the current task
 * @retval      #KERNEL_INVALID_PARAM if task_desc is a null pointer
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION InitPeriodicWait(taskDesc_t *task_desc)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (task_desc != NULL)
    {
        task_desc->last_wake = xTaskGetTickCount();
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              WaitUntilNextPeriod(taskDesc_t *task_desc)
 * @brief           Function that puts to sleep task until next period
 * @param[in,out]   task_desc Pointer to the status of the current task
 * @retval          #KERNEL_INVALID_PARAM if task_desc is a null pointer
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION WaitUntilNextPeriod(taskDesc_t *task_desc)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if (task_desc != NULL)
    {
        // Check First if a suspension is require or not
        if (task_desc->mode == TASK_SUSPENDED)
        {
            // Suspend the task
            vTaskSuspend(task_desc->handle);
        }
        else
        {
            // Before sleeping check if we missed period
            if (xTaskGetTickCount() <= (task_desc->last_wake + task_desc->period))
            {
                // If period not missed, wait until next period
                test_value = xTaskDelayUntil(&task_desc->last_wake, task_desc->period);
                if (test_value != pdTRUE)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                // Yield instead
                taskYIELD();
            }

            // Check if task has not been suspended during the sleep
            if (task_desc->mode == TASK_SUSPENDED)
            {
                // Suspend the task
                vTaskSuspend(task_desc->handle);
            }
        }

        // Update last wake time anyway
        task_desc->last_wake = xTaskGetTickCount();
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              TaskYield(const taskDesc_t *task_desc)
 * @brief           Function that yield the task
 * @param[in,out]   task_desc Pointer to the status of the current task
 * @retval          #KERNEL_INVALID_PARAM if task_desc is a null pointer
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION TaskYield(const taskDesc_t *task_desc)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (task_desc != NULL)
    {
        // Yield anyway
        taskYIELD();
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}
