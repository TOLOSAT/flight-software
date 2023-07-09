/**
 * @file tasks.c
 * @author Merlin Kooshmanian
 * @brief Source file defining tasks
 * @date 21/04/2023
 *
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tasks.h"
#include "conf/tasks_conf.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

extern taskStaticConf_t g_tasks_static_conf[NB_TASKS];
extern taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS];

/************************* Functions Definitions *****************************/

/**
 * @fn      createTasks(void)
 * @brief   Function that creates threads and links them to tasks
 * @retval  #TASK_SUCCESSFUL if creation succeed
 * @retval  #TASK_INVALID_PARAM if at least one buffer configuration is invalid (the reason could be not enough heap was defined)
 */
taskStatus_t createTasks(void)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    taskRef_t task = 0;

    // Function Core
    while (task < NB_TASKS && return_value == TASK_SUCCESSFUL)
    {
        osThreadAttr_t task_attribute = {.name=g_tasks_static_conf[task].name, .priority = g_tasks_static_conf[task].priority, .stack_size = g_tasks_static_conf[task].stack_size};
        g_tasks_dynamic_conf[task].id = osThreadNew(g_tasks_static_conf[task].handler, &g_tasks_dynamic_conf[task], &task_attribute);
        if (g_tasks_dynamic_conf[task].id == NULL)
        {
            return_value = TASK_INVALID_PARAM;
        }
        g_tasks_dynamic_conf[task].period = g_tasks_static_conf[task].default_period;
        g_tasks_dynamic_conf[task].deadline = g_tasks_static_conf[task].default_deadline;
        task++;
    }

    return (return_value);
}

/**
 * @fn          suspendTask(taskRef_t task)
 * @brief       Function that allow to suspend an active task
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #TASK_SUCCESSFUL if halt is successful
 * @retval      #TASK_ERROR if halt cannot be performed
 * @retval      #TASK_INVALID_PARAM if task ref does not exist
 */
taskStatus_t suspendTask(taskRef_t task)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSuspend(g_tasks_dynamic_conf[task].id);
        if (test_value != osOK)
        {
            return_value = TASK_ERROR;
        }
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn          resumeTask(taskRef_t task)
 * @brief       Function that allow to resume a suspended tasks
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @retval      #TASK_SUCCESSFUL if resume is successful
 * @retval      #TASK_ERROR if resume cannot be performed
 * @retval      #TASK_INVALID_PARAM if task does not exist
 */
taskStatus_t resumeTask(taskRef_t task)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadResume(g_tasks_dynamic_conf[task].id);
        if (test_value != osOK)
        {
            return_value = TASK_ERROR;
        }
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn          setTaskPriority(taskRef_t task, taskPriority_t priority)
 * @brief       Function that allows to change task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[in]   priority New priority of the task
 * @retval      #TASK_SUCCESSFUL if set is successful
 * @retval      #TASK_ERROR if set cannot be performed
 * @retval      #TASK_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
taskStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSetPriority(g_tasks_dynamic_conf[task].id, priority);
        switch (test_value)
        {
        case osOK:
            return_value = TASK_SUCCESSFUL;
            break;
        case osErrorParameter:
            return_value = TASK_INVALID_PARAM;
            break;
        default:
            return_value = TASK_ERROR;
            break;
        }
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }


    return (return_value);
}

/**
 * @fn          getTaskPriority(taskRef_t task, taskPriority_t *priority)
 * @brief       Function that allows to get task priority
 * @param[in]   task Reference of the task (in TASKS_ENUM)
 * @param[out]  priority Current priority of the task
 * @retval      #TASK_SUCCESSFUL if get is successful
 * @retval      #TASK_INVALID_PARAM if task does not exist
 * @retval      #TASK_ERROR if get cannot be performed
 */
taskStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < NB_TASKS)
    {
        *priority = osThreadGetPriority(g_tasks_dynamic_conf[task].id);
        if(*priority == osPriorityError){
            return_value = TASK_ERROR;
        }
    }
    else
    {
        return_value = TASK_INVALID_PARAM;
    }


    return (return_value);
}

/**
 * @fn          initPeriodicWait(taskDynamicConf_t *task_dyn_conf)
 * @brief       Function that init the last_wake variable in status
 * @param[in]   task_dyn_conf Pointer to the status of the current task
 * @retval      #TASK_SUCCESSFUL always
 */
taskStatus_t initPeriodicWait(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    
    // Function Core
    task_dyn_conf->last_wake =osKernelGetTickCount();

    return (return_value);
}

/**
 * @fn              waitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
 * @brief           Function that stops task until next period
 * @param[in,out]   task_dyn_conf Pointer to the status of the current task
 * @retval          #TASK_ERROR if deadline is missed
 * @retval          #TASK_SUCCESSFUL else
 */
taskStatus_t waitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf)
{
    // Variable Initialisation
    taskStatus_t return_value = TASK_SUCCESSFUL;
    
    // Function Core
    /* Before Suspension */
    if(osKernelGetTickCount() > task_dyn_conf->last_wake + task_dyn_conf->deadline)
    {
        return_value = TASK_ERROR;
    }
    osDelayUntil(task_dyn_conf->last_wake + task_dyn_conf->period);

    /* After Suspension */
    task_dyn_conf->last_wake = task_dyn_conf->last_wake + task_dyn_conf->period;
    

    return (return_value);
}