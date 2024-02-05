/**
 * @file    tasks_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration table for tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/******************************* Include Files *******************************/

#include "tasks.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    TASKS_ENUM
 * @brief   Enum defining tasks reference numbers
 */
enum TASKS_ENUM {
    MAIN_TASK = 0,
    SECOND_TASK,
    NB_TASKS,
};

/*************************** Variables Declarations **************************/

extern const taskStaticConf_t g_tasks_static_conf[NB_TASKS];
extern taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS];
extern taskStack_t g_main_task_stack[1024U];
extern taskStack_t g_second_task_stack[1024U];

#endif /* TASKS_H */