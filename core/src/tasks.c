/**
 * @file    tasks.c
 * @author  Merlin Kooshmanian
 * @brief   Source file defining tasks
 * @date    21/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core_basics.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      CreateTasks(void)
 * @brief   Function that creates threads and links them to tasks
 * @retval  #TASK_SUCCESSFUL if creation succeed
 * @retval  #TASK_ERROR if at least one task creation failed
 */
taskStatus_t CreateTasks(void)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    taskRef_t task = 0;

    // Function Core
    while ((task < (taskRef_t)NB_TASKS) && (return_value == TASK_SUCCESSFUL))
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
            return_value = TASK_ERROR;
        }
#else
        // Create task
        g_tasks_dynamic_conf[task].handle = xTaskCreateStatic( g_tasks_static_conf[task].function, 
                                            g_tasks_static_conf[task].name, 
                                            (g_tasks_static_conf[task].stack_size / sizeof(StackType_t)), 
                                            &g_tasks_dynamic_conf[task],
                                            g_tasks_static_conf[task].priority, 
                                            g_tasks_dynamic_conf[task].pointer_to_stack, 
                                            &g_tasks_dynamic_conf[task].task_control_block);
        if (g_tasks_dynamic_conf[task].handle == NULL)
        {
            return_value = TASK_ERROR;
        }
#endif 
        g_tasks_dynamic_conf[task].period = g_tasks_static_conf[task].default_period;
        g_tasks_dynamic_conf[task].deadline = g_tasks_static_conf[task].default_deadline;
        task++;
    }

    return return_value;
}

/**
 * @fn          SuspendTask(taskRef_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #TASK_SUCCESSFUL if halt is successful
 * @retval      #TASK_ERROR if halt cannot be performed
 * @retval      #TASK_INVALID_PARAM if task ref does not exist
 */
taskStatus_t SuspendTask(taskRef_t task)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        // Halt the task
        vTaskSuspend(g_tasks_dynamic_conf[task].handle);
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ResumeTask(taskRef_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #TASK_SUCCESSFUL if resume is successful
 * @retval      #TASK_ERROR if resume cannot be performed
 * @retval      #TASK_INVALID_PARAM if task does not exist
 */
taskStatus_t ResumeTask(taskRef_t task)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

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
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetTaskPriority(taskRef_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[in]   priority New priority of the task
 * @retval      #TASK_SUCCESSFUL if set is successful
 * @retval      #TASK_ERROR if set cannot be performed
 * @retval      #TASK_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
taskStatus_t SetTaskPriority(taskRef_t task, taskPriority_t priority)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        vTaskPrioritySet(g_tasks_dynamic_conf[task].handle, priority);
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetTaskPriority(taskRef_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[out]  priority Current priority of the task
 * @retval      #TASK_SUCCESSFUL if get is successful
 * @retval      #TASK_INVALID_PARAM if task does not exist
 * @retval      #TASK_ERROR if get cannot be performed
 */
taskStatus_t GetTaskPriority(taskRef_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < (taskRef_t)NB_TASKS)
    {
        *priority = uxTaskPriorityGet(g_tasks_dynamic_conf[task].handle);
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InitPeriodicWait(taskDynamicConf_t *task_dyn_conf)
 * @brief       Function that init the last_wake variable in status
 * @param[in]   task_dyn_conf Pointer to the status of the current task
 * @retval      #TASK_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval      #TASK_SUCCESSFUL else
 */
taskStatus_t InitPeriodicWait(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task_dyn_conf != NULL)
    {
        task_dyn_conf->last_wake = xTaskGetTickCount();
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              WaitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
 * @brief           Function that stops task until next period
 * @param[in,out]   task_dyn_conf Pointer to the status of the current task
 * @retval          #TASK_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval          #TASK_ERROR if deadline is missed
 * @retval          #TASK_SUCCESSFUL else
 */
taskStatus_t WaitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    BaseType_t test_value;

    // Function Core
    if (task_dyn_conf != NULL)
    {
        // Get current time
        uint32_t current_os_time = xTaskGetTickCount();

        if (task_dyn_conf->deadline != NO_DEADLINE)
        {
            // Before Suspension check deadline
            if (current_os_time <= (task_dyn_conf->last_wake + task_dyn_conf->deadline))
            {
                // If deadline not missed, wait until next period
                test_value = xTaskDelayUntil(&task_dyn_conf->last_wake, task_dyn_conf->period);
                if (test_value != pdTRUE)
                {
                    return_value = TASK_ERROR;
                }
            }
            else
            {
                return_value = TASK_ERROR;
            }
        }
        else
        {
            // Before Suspension check if we missed period
            if (current_os_time <= (task_dyn_conf->last_wake + task_dyn_conf->period))
            {
                // If deadline not missed, wait until next period
                test_value = xTaskDelayUntil(&task_dyn_conf->last_wake, task_dyn_conf->period);
                if (test_value != pdTRUE)
                {
                    return_value = TASK_ERROR;
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
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              TaskYield(const taskDynamicConf_t *task_dyn_conf)
 * @brief           Function that yield the task
 * @param[in,out]   task_dyn_conf Pointer to the status of the current task
 * @retval          #TASK_INVALID_PARAM if task_dyn_conf is a null pointer
 * @retval          #TASK_ERROR if deadline is missed
 * @retval          #TASK_SUCCESSFUL else
 */
taskStatus_t TaskYield(const taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task_dyn_conf != NULL)
    {
        // Check if deadline or not
        if (task_dyn_conf->deadline != NO_DEADLINE)
        {
            // Get current time
            uint32_t current_os_time = xTaskGetTickCount();

            // Before Yield check deadline
            if (current_os_time <= (task_dyn_conf->last_wake + task_dyn_conf->deadline))
            {
                // If deadline not missed, yield
                taskYIELD();
            }
            else
            {
                return_value = TASK_ERROR;
            }
        }
        else
        {
            // Yield anyway
            taskYIELD();
        }
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return return_value;
}
