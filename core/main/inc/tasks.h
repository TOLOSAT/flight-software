/**
 * @file tasks.h
 * @author Merlin Kooshmanian
 * @brief Header file defining tasks
 * @date 21/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TASKS_H
#define TASKS_H

/***************************** Include Files *********************************/

#include <stdint.h>
#include "cmsis_os2.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum {
    TASK_SUCCESSFUL            = 0u,    /**< Function succeed */
    TASK_ERROR                 = 1u,    /**< Function failed */
    TASK_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    TASK_TIMEOUT               = 3u,    /**< Function returned a timeout */
} tasksStatus_t;

typedef enum {
    TASK_RUNNING_AT_START       = 0u,    /**< Task will run after init */
    TASK_NOT_RUNNING_AT_START   = 1u,    /**< Task will be suspended after init */
} tasksRunOnStart_t;

typedef struct {
    uint32_t task_ref;
    osThreadFunc_t task_handler;
    void *task_handler_argument;
    osThreadAttr_t task_attribute;
    tasksRunOnStart_t task_run_on_start;
} taskDef_t;

typedef osThreadId_t taskId_t;
typedef uint32_t taskRef_t;
typedef osPriority_t taskPriority_t;

/************************** Function Prototypes ******************************/

tasksStatus_t createTasks(void);
tasksStatus_t suspendTask(taskRef_t task);
tasksStatus_t resumeTask(taskRef_t task);
tasksStatus_t setTaskPriority(taskRef_t task, taskPriority_t priority);
tasksStatus_t getTaskPriority(taskRef_t task, taskPriority_t *priority);

#endif /* TASKS_H */