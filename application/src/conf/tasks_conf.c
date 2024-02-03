/**
 * @file    tasks_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration table for tasks
 * @date    26/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/tasks_conf.h"
#include "salami/salami.h"
#include "miso/miso.h"
#include "carne/carne.h"
#include "tm_tc/tc_receiver.h"
#include "tm_tc/tm_sender.h"
#include "tm_tc/tc_scheduler.h"
#include "tm_tc/tc_process.h"
#include "housekeeping/housekeeping.h"
#include "aocs/aocs.h"
#include "power/power.h"
#include "thermal/thermal.h"
#include "gravimetry/gravimetry.h"
#include "iridium/iridium.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_tasks_static_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskStaticConf_t g_tasks_static_conf[NB_TASKS] = 
{
    /* Task Ref       , Name              , Function        , Priority              , Stack Size , Default Period , Default Deadline , Privilege       */
    { SALAMI_TASK     , "SalamiMain"      , SalamiMain      , PRIORITY_HIGH         , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { MISO_TASK       , "MisoMain"        , MisoMain        , PRIORITY_LOW          , 1024u      , 500u           , NO_DEADLINE      , TASK_PRIVILEGED },
    { CARNE_TASK      , "CarneMain"       , CarneMain       , PRIORITY_HIGH         , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { TC_RECEIVER     , "TcReceiverMain"  , TcReceiverMain  , PRIORITY_HIGH         , 2048u      , 500u           , 500u             , TASK_PRIVILEGED },
    { TM_SENDER       , "TmSenderMain"    , TmSenderMain    , PRIORITY_BELOW_NORMAL , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { TC_SCHEDULER    , "TcSchedulerMain" , TcSchedulerMain , PRIORITY_LOW          , 3072u      , 500u           , NO_DEADLINE      , TASK_PRIVILEGED },
    { TC_PROCESS      , "TcProcessMain"   , TcProcessMain   , PRIORITY_NORMAL       , 2048u      , 500u           , 500u             , TASK_PRIVILEGED },
    { HK_TASK         , "HkMain"          , HkMain          , PRIORITY_LOW          , 2048u      , 500u           , NO_DEADLINE      , TASK_PRIVILEGED },
    { AOCS_TASK       , "AocsMain"        , AocsMain        , PRIORITY_ABOVE_NORMAL , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { POWER_TASK      , "PowerMain"       , PowerMain       , PRIORITY_ABOVE_NORMAL , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { THERMAL_TASK    , "ThermalMain"     , ThermalMain     , PRIORITY_ABOVE_NORMAL , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { GRAVIMETRY_TASK , "GravimetryMain"  , GravimetryMain  , PRIORITY_NORMAL       , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
    { IRIDIUM_TASK    , "IridiumMain"     , IridiumMain     , PRIORITY_NORMAL       , 1024u      , 500u           , 500u             , TASK_PRIVILEGED },
};

/**
 * @var     g_tasks_dynamic_conf
 * @brief   Configuration table where all tasks dynamic parameters are stored
 */
taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS] = 
{
    /* Task Handle  , Task Mode     , Period , Deadline , Last Wake-Up Tick */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* SALAMI_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* MISO_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* CARNE_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_RECEIVER */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TM_SENDER */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_SCHEDULER */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* TC_PROCESS */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* HK_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* AOCS_MANAGER */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* POWER_MANAGER */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* THERMAL_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* GRAVIMETRY_TASK */
    { 0u            , TASK_NOMINAL  , 0u     , 0u       , 0u                }, /* IRIDIUM_TASK */
};
