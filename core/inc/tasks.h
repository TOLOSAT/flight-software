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
#include <FreeRTOS.h>
#include <task.h>

/***************************** Macros Definitions ****************************/

#define ANY_TASK_REF            0xffffffffu /**< Reference number to refer to any task */
#define NO_DEADLINE             0u          /**< Task does not require deadline */

#define PRIORITY_LOW            8u          /**< Low priority tasks */
#define PRIORITY_BELOW_NORMAL   16u         /**< Below normal priority tasks */
#define PRIORITY_NORMAL         24u         /**< Normal priority tasks */
#define PRIORITY_ABOVE_NORMAL   32u         /**< Above normal priority tasks */
#define PRIORITY_HIGH           40u         /**< High priority tasks */
#define PRIORITY_EXTREME        48u         /**< Extreme priority tasks */

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

/** @brief Task Handle type */
typedef TaskHandle_t taskHandle_t;

/** @brief Task Reference number type */
typedef uint32_t taskRef_t;

/** @brief Task Name type */
typedef const char taskName_t;

/** @brief Task Function type */
typedef TaskFunction_t taskFunction_t;

/** @brief Task Priority type */
typedef UBaseType_t taskPriority_t;

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
    TaskFunction_t function;            /**< @brief Task main function */
    taskPriority_t priority;            /**< @brief Task priority */
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
    taskHandle_t handle;                /**< @brief Task handle */
    taskMode_t mode;                    /**< @brief Task mode */
    taskTick_t period;                  /**< @brief Task period in ticks */
    taskTick_t deadline;                /**< @brief Task deadline in ticks */
    taskTick_t last_wake;               /**< @brief Last time the task was waken in ticks */
} taskDynamicConf_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

taskStatus_t CreateTasks(void);
taskStatus_t SuspendTask(taskRef_t task);
taskStatus_t ResumeTask(taskRef_t task);
taskStatus_t SetTaskPriority(taskRef_t task, taskPriority_t priority);
taskStatus_t GetTaskPriority(taskRef_t task, taskPriority_t *priority);
taskStatus_t InitPeriodicWait(taskDynamicConf_t *task_dyn_conf);
taskStatus_t WaitUntilNextPeriod(taskDynamicConf_t *task_dyn_conf);
taskStatus_t TaskYield(taskDynamicConf_t *task_dyn_conf);

#endif /* TASKS_H */

/** 
 * @} 
 * @} 
 */