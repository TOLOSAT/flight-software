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

/**
 * @var     g_tasks_conf
 * @brief   Configuration table that defining all tasks
 */
const taskDef_t g_tasks_conf[NB_TASKS] = 
{
    /* Task Ref   , Handler       , Handler Argument , Priority              , Stack Size , RUN AT START YES/NO      */
    {MAIN_TASK    , DummyMainTask , NULL             , osPriorityAboveNormal , 1024U      , TASK_RUNNING_AT_START     },
    {BLINK01_TASK , StartBlink01  , NULL             , osPriorityNormal      , 1024U      , TASK_NOT_RUNNING_AT_START },
    {BLINK02_TASK , StartBlink02  , NULL             , osPriorityBelowNormal , 1024U      , TASK_NOT_RUNNING_AT_START },
};

/**
 * @var     g_tasks_ids
 * @brief   Table that will stock all task ids
 */
taskId_t g_tasks_ids[NB_TASKS];