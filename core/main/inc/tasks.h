/**
 * @file tasks.h
 * @author Merlin Kooshmanian
 * @brief Header file defining tasks
 * @date 21/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup tasks Tasks Management
 * Functions that allows to manage tasks in TAPAS
 * @{
 */

#ifndef TASKS_H
#define TASKS_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "cmsis_os2.h"

/************************** Constant Definitions *****************************/

#define ANY_TASK_REF        0xffffffff  /**< Reference number to refer to any task */

/**************************** Type Definitions *******************************/

/** 
 * @typedef tasksStatus_t
 * @brief   Task functions specific returns 
 */
typedef enum
{
    TASK_SUCCESSFUL            = 0u,    /**< Function succeed */
    TASK_ERROR                 = 1u,    /**< Function failed */
    TASK_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    TASK_TIMEOUT               = 3u,    /**< Function returned a timeout */
} tasksStatus_t;

/** 
 * @typedef tasksRunOnStart_t
 * @brief   Type enum defining if task starts after initialisation
 */
typedef enum
{
    TASK_RUNNING_AT_START       = 0u,    /**< Task will run after init */
    TASK_NOT_RUNNING_AT_START   = 1u,    /**< Task will be suspended after init */
} tasksRunOnStart_t;

/** @brief Task ID type */
typedef osThreadId_t taskId_t;

/** @brief Task Reference number type */
typedef uint32_t taskRef_t;

/** @brief Task Priority type */
typedef osPriority_t taskPriority_t;

/** @brief Task Stack Size type */
typedef uint32_t taskStackSize_t;

/** 
 * @typedef taskDef_t
 * @struct  taskDef_t
 * @brief   Struct type definition of a task
 */
typedef struct
{                            
    uint32_t ref;                       /**< @brief Task reference number as it is declared in TASKS_ENUM */
    osThreadFunc_t handler;             /**< @brief Task handling function */
    void *handler_argument;             /**< @brief Task handling function argument */
    taskPriority_t priority;            /**< @brief Task priority as defined in cmsis_os2.h */
    taskStackSize_t stack_size;         /**< @brief Task stack size in bits */
    tasksRunOnStart_t run_on_start;     /**< @brief Define if the task starts after initialisation */
} taskDef_t;

/************************** Function Prototypes ******************************/

tasksStatus_t createTasks(void);
tasksStatus_t suspendTask(taskRef_t task);
tasksStatus_t resumeTask(taskRef_t task);
tasksStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority);
tasksStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority);

#endif /* TASKS_H */

/** @} */