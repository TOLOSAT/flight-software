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

extern taskDef_t g_tasks_conf[NB_TASKS];
extern taskId_t g_tasks_ids[NB_TASKS];

/************************* Functions Definitions *****************************/

/**
 * @fn      createTasks(void)
 * @brief   Function that creates threads and links them to tasks
 * @param   void
 * @retval  TASKS_SUCCESSFUL if creation succeed
 * @retval  TASKS_INVALID_PARAM if one tasks was wrongly setup
 */
tasksStatus_t createTasks(void)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    taskRef_t task = 0;

    // Function Core
    while (task < NB_TASKS && return_value == TASK_SUCCESSFUL)
    {
        g_tasks_ids[task] = osThreadNew(g_tasks_conf[task].task_handler, g_tasks_conf[task].task_handler_argument, &g_tasks_conf[task].task_attribute);
        if (g_tasks_ids[task] == NULL)
        {
            return_value = TASK_INVALID_PARAM;
        }
        if (g_tasks_conf[task].task_run_on_start == TASK_NOT_RUNNING_AT_START)
        {
            suspendTask(task);
        }
        task++;
    }

    return (return_value);
}

/**
 * @fn      suspendTask(void)
 * @brief   Function that allow to suspend an active task
 * @param   task Reference of the task (in TASKS_ENUM)
 * @retval  TASKS_SUCCESSFUL if halt is successful
 * @retval  TASKS_ERROR if halt cannot be performed
 * @retval  TASKS_INVALID_PARAM if task ref does not exist
 */
tasksStatus_t suspendTask(taskRef_t task)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSuspend(g_tasks_ids[task]);
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
 * @fn      resumeTask(void)
 * @brief   Function that allow to resume a suspended tasks
 * @param   task Reference of the task (in TASKS_ENUM)
 * @retval  TASKS_SUCCESSFUL if resume is successful
 * @retval  TASKS_ERROR if resume cannot be performed
 * @retval  TASKS_INVALID_PARAM if task does not exist
 */
tasksStatus_t resumeTask(taskRef_t task)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadResume(g_tasks_ids[task]);
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
 * @retval  TASKS_SUCCESSFUL if set is successful
 * @retval  TASKS_ERROR if set cannot be performed
 * @retval  TASKS_INVALID_PARAM if task does not exist
 * @retval  TASKS_INVALID_PARAM if priority < IDLE or priority > ISR
 */
tasksStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;
    osStatus_t test_value;

    // Function Core
    if (task < NB_TASKS)
    {
        test_value = osThreadSetPriority(g_tasks_ids[task], priority);
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
 * @retval  TASKS_SUCCESSFUL if get is successful
 * @retval  TASKS_ERROR if get cannot be performed
 */
tasksStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority)
{
    // Variable Initialisation
    tasksStatus_t return_value = TASK_SUCCESSFUL;

    // Function Core
    if (task < NB_TASKS)
    {
        *priority = osThreadGetPriority(g_tasks_ids[task]);
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
