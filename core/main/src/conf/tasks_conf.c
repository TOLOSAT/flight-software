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

const taskDef_t g_tasks_conf[NB_TASKS] = {
    {BLINK01_TASK, StartBlink01, NULL, {.priority = osPriorityNormal     , .stack_size = 1024}},
    {BLINK02_TASK, StartBlink02, NULL, {.priority = osPriorityBelowNormal, .stack_size = 1024}},
    {BLINK03_TASK, StartBlink03, NULL, {.priority = osPriorityBelowNormal, .stack_size = 1024}},
};

taskId_t g_tasks_ids[NB_TASKS];