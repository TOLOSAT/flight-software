/**
 * @file    tasks.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining tasks
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup tasks Tasks
 * @brief Tasks handling interface.
 * @{
 */

#ifndef TASKS_H
#define TASKS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     TASKNO_TO_LINENO(taskno)
 * @brief   Convert a task number to the corresponding line number in the tasks table.
 */
#define TASKNO_TO_LINENO(task_no) ((task_no) - 1u)

/***************************** Types Definitions *****************************/

/**
 * @enum    taskMode_t
 * @brief   Task Modes
 */
typedef enum
{
    TASK_SUSPENDED = 0u, /**< Task is in SUSPENDED mode */
    TASK_SAFE      = 1u, /**< Task is in SAFE mode */
    TASK_NOMINAL   = 2u, /**< Task is in NOMINAL mode */
} taskMode_t;

/**
 * @enum    taskPrivilege_t
 * @brief   Task Privilege
 */
typedef enum
{
    TASK_UNPRIVILEGED = 0u, /**< Task is not priviledged */
    TASK_PRIVILEGED   = 1u, /**< Task is priviledged */
} taskPrivilege_t;

/** @brief Task Handle type */
typedef TaskHandle_t taskHandle_t;

/** @brief Task Name type */
typedef const char taskName_t;

/** @brief Task Function type */
typedef TaskFunction_t taskFunction_t;

/** @brief Task Stack Size type */
typedef uint32_t taskStackSize_t;

/** @brief Task stack type */
typedef StackType_t taskStack_t;

/** @brief Task Control Block (TCB) type */
typedef StaticTask_t taskTCB_t;

/**
 * @struct  taskConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{
    taskNo_t task;              /**< @brief Task reference number as it is declared in TASKS_ENUM */
    taskName_t *name;           /**< @brief Task name only for debugging purposes */
    taskFunction_t function;    /**< @brief Task main function */
    taskPriority_t priority;    /**< @brief Task priority */
    taskStackSize_t stack_size; /**< @brief Task stack size in bits */
    tick_t default_period;      /**< @brief Task default period in ticks */
    taskPrivilege_t privilege;  /**< @brief Task privilege (applicable only if the MPU is activated) */
    taskTCB_t *p_tcb;           /**< @brief Pointer to task control block  */
    taskStack_t *p_stack;       /**< @brief Pointer to task stack */
} taskConf_t;

/**
 * @struct  taskDesc_t
 * @brief   Struct type of a task descriptors
 */
typedef struct
{
    taskHandle_t handle;     /**< @brief Task handle */
    taskMode_t mode;         /**< @brief Task mode */
    tick_t period;           /**< @brief Task period in ticks */
    tick_t last_wake;        /**< @brief Last time the task was waken in ticks */
    uint32_t syscall_tmp_lr; /**< @brief Store temporarily the LR when executing a syscall */
} taskDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_tasks_conf_table
 * @brief   Configuration table where all tasks static parameters are stored
 */
extern const taskConf_t g_tasks_conf_table[CONFIG_MAX_NB_TASKS];

/**
 * @var     g_tasks_desc_table
 * @brief   Configuration table where all tasks descriptors are stored
 */
extern taskDesc_t g_tasks_desc_table[CONFIG_MAX_NB_TASKS];

/*************************** Functions Declarations **************************/

extern void CreateTasks(void);
extern taskNo_t GetCurrentTask(void);
extern returnCode_t SuspendTask(taskNo_t task);
extern returnCode_t ResumeTask(taskNo_t task);
extern returnCode_t GetTaskPriority(taskNo_t task, taskPriority_t *priority);
extern returnCode_t SetTaskPriority(taskNo_t task, taskPriority_t priority);

#endif /* TASKS_H */

/**
 * @}
 * @}
 * @}
 */