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

extern taskConf_t g_tasks_conf[NB_TASKS];
extern taskStatus_t g_tasks_status[NB_TASKS];

/************************* Functions Definitions *****************************/

/**
 * @fn      createTasks(void)
 * @brief   Function that creates threads and links them to tasks
 * @retval  TASK_SUCCESSFUL if creation succeed
 * @retval  TASK_INVALID_PARAM if at least one buffer configuration is invalid (the reason could be not enough heap was defined)
 */
tasksStatus_t createTasks(void)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    taskRef_t task = 0;

    // Function Core
    while (task < NB_TASKS && return_value == TASK_SUCCESSFUL)
    {
        osThreadAttr_t task_attribute = {.name=g_tasks_conf[task].name, .priority = g_tasks_conf[task].priority, .stack_size = g_tasks_conf[task].stack_size};
        g_tasks_status[task].id = osThreadNew(g_tasks_conf[task].handler, &g_tasks_status[task], &task_attribute);
        if (g_tasks_status[task].id == NULL)
        {
            return_value = TASK_INVALID_PARAM;
        }
        g_tasks_status[task].period = g_tasks_conf[task].default_period;
        if (g_tasks_conf[task].run_on_start == TASK_NOT_RUNNING_AT_START)
        {
            suspendTask(task);
        }
        task++;
    }

    return (return_value);
}

/**
 * @fn      suspendTask(taskRef_t task)
 * @brief   Function that allow to suspend an active task
 * @param   task Reference of the task (in TASKS_ENUM)
 * @retval  TASK_SUCCESSFUL if halt is successful
 * @retval  TASK_ERROR if halt cannot be performed
 * @retval  TASK_INVALID_PARAM if task ref does not exist
 */
tasksStatus_t suspendTask(taskRef_t task)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSuspend(g_tasks_status[task].id);
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
 * @fn      resumeTask(taskRef_t task)
 * @brief   Function that allow to resume a suspended tasks
 * @param   task Reference of the task (in TASKS_ENUM)
 * @retval  TASK_SUCCESSFUL if resume is successful
 * @retval  TASK_ERROR if resume cannot be performed
 * @retval  TASK_INVALID_PARAM if task does not exist
 */
tasksStatus_t resumeTask(taskRef_t task)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadResume(g_tasks_status[task].id);
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
 * @fn      setTaskPriority(taskRef_t task, taskPriority_t priority)
 * @brief   Function that allows to change task priority
 * @param   task Reference of the task (in TASKS_ENUM)
 * @param   priority New priority of the task
 * @retval  TASK_SUCCESSFUL if set is successful
 * @retval  TASK_ERROR if set cannot be performed
 * @retval  TASK_INVALID_PARAM if task does not exist or if priority < IDLE or priority > ISR
 */
tasksStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSetPriority(g_tasks_status[task].id, priority);
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
 * @fn      getTaskPriority(taskRef_t task, taskPriority_t *priority)
 * @brief   Function that allows to get task priority
 * @param   task Reference of the task (in TASKS_ENUM)
 * @param   priority Current priority of the task
 * @retval  TASK_SUCCESSFUL if get is successful
 * @retval  TASK_INVALID_PARAM if task does not exist
 * @retval  TASK_ERROR if get cannot be performed
 */
tasksStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < NB_TASKS)
    {
        *priority = osThreadGetPriority(g_tasks_status[task].id);
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
 * @fn      initPeriodicWait(taskStatus_t *current_status)
 * @brief   Function that init the last_wake variable in status
 * @param   current_status Pointer to the status of the current task
 * @retval  TASK_SUCCESSFUL always
 */
tasksStatus_t initPeriodicWait(taskStatus_t *current_status)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    
    // Function Core
    current_status->last_wake =osKernelGetTickCount();

    return (return_value);
}

/**
 * @fn      waitUntilNextPeriod(taskStatus_t *current_status)
 * @brief   Function that stops task until next period
 * @param   current_status Pointer to the status of the current task
 * @retval  TASK_SUCCESSFUL always
 */
tasksStatus_t waitUntilNextPeriod(taskStatus_t *current_status)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    
    // Function Core
    osDelayUntil(current_status->last_wake + current_status->period);
    current_status->last_wake =osKernelGetTickCount();

    return (return_value);
}