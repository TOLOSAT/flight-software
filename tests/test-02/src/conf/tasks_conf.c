/**
 * @file tasks_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking configuration table for tasks
 * @date 02/07/2023
 * 
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "conf/tasks_conf.h"
#include "tm_tc/tc_receiver.h"
#include "tm_tc/tm_sender.h"
#include "tm_tc/tc_scheduler.h"
#include "tm_tc/tc_process.h"

/***************************** Configuration *********************************/

/**
 * @var     g_tasks_static_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskStaticConf_t g_tasks_static_conf[NB_TASKS] = 
{
    /* Task Ref      , Name              , Handler         , Priority              , Stack Size , Default Period , Default Deadline */
    {TC_RECEIVER     , "TcReceiverMain"  , TcReceiverMain  , osPriorityHigh        , 1024u      , 500u           , 500u             },
    {TM_SENDER       , "TmSenderMain"    , TmSenderMain    , osPriorityLow         , 1024u      , 500u           , 500u             },
    {TC_SCHEDULER    , "TcSchedulerMain" , TcSchedulerMain , osPriorityNormal      , 1024u      , 500u           , 500u             },
    {TC_PROCESS      , "TcProcessMain"   , TcProcessMain   , osPriorityNormal      , 1024u      , 500u           , 500u             },
};

/**
 * @var     g_tasks_dynamic_conf
 * @brief   Configuration table where all tasks dynamic parameters are stored
 */
taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS] = 
{
    /* Task ID , Task Mode     , Period , Deadline , Last Wake-Up Tick */
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_RECEIVER */
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TM_SENDER */
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_SCHEDULER */
    {0u        , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_PROCESS */
};
