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
const taskConf_t g_tasks_conf[NB_TASKS] = 
{
    /* Task Ref   , Name       , Handler       , Priority              , Stack Size , Default Period , Default Deadline */
    {MAIN_TASK    , "MainTask" , DummyMainTask , osPriorityAboveNormal , 1024U      , 800u           , 800u              },
    {BLINK01_TASK , "Blink01"  , StartBlink01  , osPriorityNormal      , 1024U      , 500u           , 500u              },
    {BLINK02_TASK , "Blink02"  , StartBlink02  , osPriorityBelowNormal , 1024U      , 1100u          , 1100u             },
};

/**
 * @var     g_tasks_status
 * @brief   Status table where all tasks status (id, mode, last wake-up, ...) are stored
 */
taskStatus_t g_tasks_status[NB_TASKS] = 
{
    /* Task ID , Task Mode     , Period , Deadline , Last Wake-Up Tick */
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                 },
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                 },
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                 },
};
