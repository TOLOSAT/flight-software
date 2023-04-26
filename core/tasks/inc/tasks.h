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

#define TASKS_SUCCESSFUL            0 /**< Function succeed */
#define TASKS_ERROR                 1 /**< Function failed*/
#define TASKS_INVALID_PARAM         2 /**< Function parameter is not valid*/
#define TASKS_TIMEOUT               3 /**< Function returned a timeout*/

/**************************** Type Definitions *******************************/

typedef struct {
    osThreadFunc_t task_handler;
    void *handler_argument;
    osThreadAttr_t task_attribute;
} taskDef_t;

typedef osThreadId_t taskId_t;

typedef uint32_t taskRef_t;

/************************** Function Prototypes ******************************/

uint32_t createTasks(void);

#endif /* TASKS_H */