/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 * @date    21/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core_basics.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that creates threads and links them to tasks
 * @retval  #CORE_SUCCESSFUL if creation succeed
 * @retval  #CORE_ERROR if at least one task creation failed
 */
coreStatus_t IN_CORE_TEXT_SECTION CreateTasks(void)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    taskRef_t task = 0;

    // Function Core
    while ((task < (taskRef_t)NB_TASKS) && (return_value == CORE_SUCCESSFUL))
    {
#if defined(MPU_AVAILABLE)
        BaseType_t test_value = pdPASS;
        TaskParameters_t task_parameters =
            {
                .pvTaskCode = g_tasks_static_conf[task].function,
                .pcName = g_tasks_static_conf[task].name,
                .usStackDepth = (g_tasks_static_conf[task].stack_size / sizeof(StackType_t)),
                .pvParameters = &g_tasks_dynamic_conf[task],
                .uxPriority = g_tasks_static_conf[task].priority,
                .puxStackBuffer = g_tasks_dynamic_conf[task].pointer_to_stack,
                .pxTaskBuffer = &g_tasks_dynamic_conf[task].task_control_block,
            };
        // Add Privileged bit if task is privileged
        if (g_tasks_static_conf[task].privilege == TASK_PRIVILEGED)
        {
            task_parameters.uxPriority |= portPRIVILEGE_BIT;
        }
        // Create task
        test_value = xTaskCreateRestrictedStatic(&task_parameters, &g_tasks_dynamic_conf[task].handle);
        if (test_value != pdPASS)
        {
            return_value = CORE_ERROR;
        }
#else
        // Create task
        g_tasks_dynamic_conf[task].handle = xTaskCreateStatic(g_tasks_static_conf[task].function,
                                                              g_tasks_static_conf[task].name,
                                                              (g_tasks_static_conf[task].stack_size / sizeof(StackType_t)),
                                                              &g_tasks_dynamic_conf[task],
                                                              g_tasks_static_conf[task].priority,
                                                              g_tasks_dynamic_conf[task].pointer_to_stack,
                                                              &g_tasks_dynamic_conf[task].task_control_block);
        if (g_tasks_dynamic_conf[task].handle == NULL)
        {
            return_value = CORE_ERROR;
        }
#endif
        g_tasks_dynamic_conf[task].period = g_tasks_static_conf[task].default_period;
        task++;
    }

    return return_value;
}

/**
 * @fn          ResetTask(taskRef_t task)
 * @brief       Function resets the chosen task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #CORE_SUCCESSFUL if halt is successful
 * @retval      #CORE_ERROR if halt cannot be performed
 * @retval      #CORE_INVALID_PARAM if task ref does not exist
 */
coreStatus_t IN_CORE_TEXT_SECTION ResetTask(taskRef_t task)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        // First release all holded mutexes
        return_value = ResetHoldedMutexes(task);
        if (return_value == CORE_SUCCESSFUL)
        {
            // Then reset the task

            // Entering in the critical section because
            // this action cannot be preempted.
            taskENTER_CRITICAL();

            // First delete task and erase content
            vTaskDelete(g_tasks_dynamic_conf[task].handle);
            (void)memset(&g_tasks_dynamic_conf[task].handle, 0, sizeof(taskHandle_t));
            (void)memset(g_tasks_dynamic_conf[task].pointer_to_stack, 0, g_tasks_static_conf[task].stack_size);

            // Then recreate the task
#if defined(MPU_AVAILABLE)
            BaseType_t test_value = pdPASS;
            TaskParameters_t task_parameters =
                {
                    .pvTaskCode = g_tasks_static_conf[task].function,
                    .pcName = g_tasks_static_conf[task].name,
                    .usStackDepth = (g_tasks_static_conf[task].stack_size / sizeof(StackType_t)),
                    .pvParameters = &g_tasks_dynamic_conf[task],
                    .uxPriority = g_tasks_static_conf[task].priority,
                    .puxStackBuffer = g_tasks_dynamic_conf[task].pointer_to_stack,
                    .pxTaskBuffer = &g_tasks_dynamic_conf[task].task_control_block,
                };
            // Add Privileged bit if task is privileged
            if (g_tasks_static_conf[task].privilege == TASK_PRIVILEGED)
            {
                task_parameters.uxPriority |= portPRIVILEGE_BIT;
            }
            // Create task
            test_value = xTaskCreateRestrictedStatic(&task_parameters, &g_tasks_dynamic_conf[task].handle);
            if (test_value != pdPASS)
            {
                return_value = CORE_ERROR;
            }
#else
            // Create task
            g_tasks_dynamic_conf[task].handle = xTaskCreateStatic(g_tasks_static_conf[task].function,
                                                                  g_tasks_static_conf[task].name,
                                                                  (g_tasks_static_conf[task].stack_size / sizeof(StackType_t)),
                                                                  &g_tasks_dynamic_conf[task],
                                                                  g_tasks_static_conf[task].priority,
                                                                  g_tasks_dynamic_conf[task].pointer_to_stack,
                                                                  &g_tasks_dynamic_conf[task].task_control_block);
            if (g_tasks_dynamic_conf[task].handle == NULL)
            {
                return_value = CORE_ERROR;
            }
#endif
            g_tasks_dynamic_conf[task].period = g_tasks_static_conf[task].default_period;

            // Come back to normal execution
            taskEXIT_CRITICAL();
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SuspendTask(taskRef_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #CORE_SUCCESSFUL if halt is successful
 * @retval      #CORE_ERROR if cannot release task's mutexes
 * @retval      #CORE_INVALID_PARAM if task ref does not exist
 */
coreStatus_t IN_CORE_TEXT_SECTION SuspendTask(taskRef_t task)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        // First release all holded mutexes
        return_value = ResetHoldedMutexes(task);
        if (return_value == CORE_SUCCESSFUL)
        {
            // Halt the task
            vTaskSuspend(g_tasks_dynamic_conf[task].handle);
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ResumeTask(taskRef_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #CORE_SUCCESSFUL if resume is successful
 * @retval      #CORE_INVALID_PARAM if task does not exist
 */
coreStatus_t IN_CORE_TEXT_SECTION ResumeTask(taskRef_t task)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        // Resume the task
        vTaskResume(g_tasks_dynamic_conf[task].handle);

        // Get current time
        uint32_t current_os_time = xTaskGetTickCount();

        // Update Last Wake Time for the task
        g_tasks_dynamic_conf[task].last_wake = current_os_time;
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetTaskPriority(taskRef_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[in]   priority New priority of the task
 * @retval      #CORE_SUCCESSFUL if set is successful
 * @retval      #CORE_ERROR if set cannot be performed
 * @retval      #CORE_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
coreStatus_t IN_CORE_TEXT_SECTION SetTaskPriority(taskRef_t task, taskPriority_t priority)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        vTaskPrioritySet(g_tasks_dynamic_conf[task].handle, priority);
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetTaskPriority(taskRef_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[out]  priority Current priority of the task
 * @retval      #CORE_SUCCESSFUL if get is successful
 * @retval      #CORE_INVALID_PARAM if task does not exist
 * @retval      #CORE_ERROR if get cannot be performed
 */
coreStatus_t IN_CORE_TEXT_SECTION GetTaskPriority(taskRef_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        *priority = uxTaskPriorityGet(g_tasks_dynamic_conf[task].handle);
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InitPeriodicWait(taskDynamicConf_t *task_dyn_conf)
 * @brief       Function that init the last_wake variable in status
 * @param[in]   task_dyn_conf Pointer to the status of the current task
 * @retval      #CORE_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION InitPeriodicWait(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task_dyn_conf != NULL)
    {
        task_dyn_conf->last_wake = xTaskGetTickCount();
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              WaitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
 * @brief           Function that stops task until next period
 * @param[in,out]   task_dyn_conf Pointer to the status of the current task
 * @retval          #CORE_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval          #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION WaitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if (task_dyn_conf != NULL)
    {
        // Get current time
        uint32_t current_os_time = xTaskGetTickCount();

        // Before Suspension check if we missed period
        if (current_os_time <= (task_dyn_conf->last_wake + task_dyn_conf->period))
        {
            // If period not missed, wait until next period
            test_value = xTaskDelayUntil(&task_dyn_conf->last_wake, task_dyn_conf->period);
            if (test_value != pdTRUE)
            {
                return_value = CORE_ERROR;
            }
        }
        else
        {
            // Yield instead
            taskYIELD();

            // After yield update last wake with current os time
            task_dyn_conf->last_wake = xTaskGetTickCount();
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              TaskYield(const taskDynamicConf_t *task_dyn_conf)
 * @brief           Function that yield the task
 * @param[in,out]   task_dyn_conf Pointer to the status of the current task
 * @retval          #CORE_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval          #CORE_SUCCESSFUL else
 */
coreStatus_t IN_CORE_TEXT_SECTION TaskYield(const taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if (task_dyn_conf != NULL)
    {

        // Yield anyway
        taskYIELD();
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}
