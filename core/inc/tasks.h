/**
 * @file    tasks.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining tasks
 * @date    21/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup tasks Tasks Management
 * Functions that allows to manage tasks in TAPAS
 * @{
 */

#ifndef TASKS_H
#define TASKS_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "cmsis_os2.h"

/***************************** Macros Definitions ****************************/

#define ANY_TASK_REF        0xffffffffu /**< Reference number to refer to any task */

/***************************** Types Definitions *****************************/

/** 
 * @enum    taskStatus_t
 * @brief   Task functions specific returns 
 */
typedef enum
{
    TASK_SUCCESSFUL            = 0u,    /**< Function succeed */
    TASK_ERROR                 = 1u,    /**< Function failed */
    TASK_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    TASK_TIMEOUT               = 3u,    /**< Function returned a timeout */
} taskStatus_t;

/** 
 * @enum    taskMode_t
 * @brief   Task Modes
 */
typedef enum
{
    TASK_HALTED                 = 0u,    /**< Task is in HALTED mode */
    TASK_SAFE                   = 1u,    /**< Task is in SAFE mode */
    TASK_NOMINAL                = 2u,    /**< Task is in NOMINAL mode */
} taskMode_t;


/** @brief Task ID type */
typedef osThreadId_t taskId_t;

/** @brief Task Reference number type */
typedef uint32_t taskRef_t;

/** @brief Task Name type */
typedef const char taskName_t;

/** @brief Task Priority type */
typedef osPriority_t taskPriority_t;

/** @brief Task Stack Size type */
typedef uint32_t taskStackSize_t;

/** @brief Task tick type */
typedef uint32_t taskTick_t;

/** 
 * @struct  taskStaticConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{                            
    taskRef_t ref;                      /**< @brief Task reference number as it is declared in TASKS_ENUM */
    taskName_t *name;                   /**< @brief Task name only for debugging purposes */
    osThreadFunc_t handler;             /**< @brief Task handling function */
    taskPriority_t priority;            /**< @brief Task priority as defined in cmsis_os2.h */
    taskStackSize_t stack_size;         /**< @brief Task stack size in bits */
    taskTick_t default_period;          /**< @brief Task default period in ticks */
    taskTick_t default_deadline;        /**< @brief Task default deadline in ticks */
} taskStaticConf_t;

/** 
 * @struct  taskDynamicConf_t
 * @brief   Struct type of a task dynamic parameters
 */
typedef struct
{                            
    taskId_t id;                        /**< @brief Task id */
    taskMode_t mode;                    /**< @brief Task mode */
    taskTick_t period;                  /**< @brief Task period in ticks */
    taskTick_t deadline;                /**< @brief Task deadline in ticks */
    taskTick_t last_wake;               /**< @brief Last time the task was waken in ticks */
} taskDynamicConf_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

taskStatus_t createTasks(void);
taskStatus_t suspendTask(taskRef_t task);
taskStatus_t resumeTask(taskRef_t task);
taskStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority);
taskStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority);
taskStatus_t initPeriodicWait(taskDynamicConf_t *task_dyn_conf);
taskStatus_t waitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf);

#endif /* TASKS_H */

/** 
 * @} 
 * @} 
 */