/**
 * @file tasks_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking configuration table for tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "conf/tasks_conf.h"
#include "dummy_tasks.h"

/***************************** Configuration *********************************/

const taskDef_t g_normal_tasks[NB_TASKS] = {
    {StartBlink01, NULL, {.name = "blink01", .priority = (osPriority_t)osPriorityNormal     , .stack_size = 128}},
    {StartBlink02, NULL, {.name = "blink02", .priority = (osPriority_t)osPriorityBelowNormal, .stack_size = 128}},
};

taskId_t g_normal_tasks_ids[NB_TASKS];