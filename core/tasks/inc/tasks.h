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
    TASKS_SUCCESSFUL        = 0u,    /**< Function succeed */
    TASKS_ERROR             = 1u,    /**< Function failed*/
    TASKS_INVALID_PARAM     = 2u,    /**< Function parameter is not valid**/
    TASKS_TIMEOUT           = 3u,    /**< Function returned a timeout*/
} tasksStatus_t;

typedef struct {
    uint32_t task_ref;
    osThreadFunc_t task_handler;
    void *task_handler_argument;
    osThreadAttr_t task_attribute;
} taskDef_t;

typedef osThreadId_t taskId_t;

typedef uint32_t taskRef_t;

/************************** Function Prototypes ******************************/

tasksStatus_t createTasks(void);
tasksStatus_t suspendTask(taskRef_t task);
tasksStatus_t resumeTask(taskRef_t task);

#endif /* TASKS_H */