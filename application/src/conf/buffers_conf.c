/**
 * @file    buffers_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration table for buffers
 * @date    26/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "pus_constants.h"
#include "life.h"
#include "events.h"
#include "housekeeping.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_buffers_static_conf
 * @brief   Configuration table where all buffers static parameters are stored
 */
const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS] = 
{
    /* Buffer Ref       , Sender Ref      , Receiver Ref    , Msg Size                  , Msg Nb */
    { MISO_LIFE         , MISO_TASK       , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { CARNE_LIFE        , CARNE_LIFE      , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { TC_RECEIVER_LIFE  , TC_RECEIVER     , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { TM_SENDER_LIFE    , TM_SENDER       , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { TC_SCHEDULER_LIFE , TC_SCHEDULER    , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { TC_PROCESS_LIFE   , TC_PROCESS      , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { HK_LIFE           , HK_TASK         , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { AOCS_LIFE         , AOCS_TASK       , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { POWER_LIFE        , POWER_TASK      , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { THERMAL_LIFE      , THERMAL_TASK    , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { GRAVIMETRY_LIFE   , GRAVIMETRY_TASK , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { IRIDIUM_LIFE      , IRIDIUM_TASK    , SALAMI_TASK     , LIFE_MESSAGE_SIZE         , 3U     },
    { MISO_MODE_CHANGE  , MISO_TASK       , SALAMI_TASK     , 10U                       , 3U     },
    { CARNE_MODE_CHANGE , CARNE_TASK      , SALAMI_TASK     , 10U                       , 3U     },
    { SALAMI_EVENTS     , SALAMI_TASK     , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { MISO_EVENTS       , MISO_TASK       , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { AOCS_EVENTS       , AOCS_TASK       , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { POWER_EVENTS      , POWER_TASK      , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { THERMAL_EVENTS    , THERMAL_TASK    , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { GRAVIMETRY_EVENTS , GRAVIMETRY_TASK , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { IRIDIUM_EVENTS    , IRIDIUM_TASK    , CARNE_TASK      , EVENT_MESSAGE_SIZE        , 3U     },
    { AOCS_HK           , AOCS_TASK       , HK_TASK         , HOUSEKEEPING_MESSAGE_SIZE , 3U     },
    { POWER_HK          , POWER_TASK      , HK_TASK         , HOUSEKEEPING_MESSAGE_SIZE , 3U     },
    { THERMAL_HK        , THERMAL_TASK    , HK_TASK         , HOUSEKEEPING_MESSAGE_SIZE , 3U     },
    { GRAVIMETRY_HK     , GRAVIMETRY_HK   , HK_TASK         , HOUSEKEEPING_MESSAGE_SIZE , 3U     },
    { IRIDIUM_HK        , IRIDIUM_HK      , HK_TASK         , HOUSEKEEPING_MESSAGE_SIZE , 3U     },
    { TC_NORMAL         , TC_RECEIVER     , TC_PROCESS      , TC_MAX_SIZE               , 4U     },
    { TC_PUS3           , TC_RECEIVER     , HK_TASK         , TC_MAX_SIZE               , 4U     },
    { TC_PUS5           , TC_RECEIVER     , CARNE_TASK      , TC_MAX_SIZE               , 4U     },
    { TC_PUS11          , TC_RECEIVER     , TC_SCHEDULER    , TC_MAX_SIZE               , 4U     },
    { TC_PUS160         , TC_RECEIVER     , SALAMI_TASK     , TC_MAX_SIZE               , 4U     },
    { TC_PUS161         , TC_RECEIVER     , MISO_TASK       , TC_MAX_SIZE               , 4U     },
    { TC_PUS176         , TC_RECEIVER     , AOCS_TASK       , TC_MAX_SIZE               , 4U     },
    { TC_PUS177         , TC_RECEIVER     , POWER_TASK      , TC_MAX_SIZE               , 4U     },
    { TC_PUS178         , TC_RECEIVER     , THERMAL_TASK    , TC_MAX_SIZE               , 4U     },
    { TC_PUS192         , TC_RECEIVER     , GRAVIMETRY_TASK , TC_MAX_SIZE               , 4U     },
    { TC_PUS193         , TC_RECEIVER     , IRIDIUM_TASK    , TC_MAX_SIZE               , 4U     },
    { TC_DELAYED        , TC_SCHEDULER    , TC_RECEIVER     , TC_MAX_SIZE               , 4U     },
    { TM_NORMAL         , TC_PROCESS      , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS1           , ANY_TASK_REF    , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS3           , HK_TASK         , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS5           , CARNE_TASK      , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS160         , SALAMI_TASK     , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS161         , MISO_TASK       , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS176         , AOCS_TASK       , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS177         , POWER_TASK      , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS178         , THERMAL_TASK    , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS192         , GRAVIMETRY_TASK , TM_SENDER       , TM_MAX_SIZE               , 4U     },
    { TM_PUS193         , IRIDIUM_TASK    , TM_SENDER       , TM_MAX_SIZE               , 4U     },
};

/**
 * @var     g_buffers_dynamic_conf
 * @brief   Configuration table where all buffers dynamic parameters are stored
 */
bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS] = 
{
    /* Buffer ID , Nb Msg */
    { 0u         , 0u     }, /* MISO_LIFE */
    { 0u         , 0u     }, /* CARNE_LIFE */
    { 0u         , 0u     }, /* TC_RECEIVER_LIFE */
    { 0u         , 0u     }, /* TM_SENDER_LIFE */
    { 0u         , 0u     }, /* TC_SCHEDULER_LIFE */
    { 0u         , 0u     }, /* TC_PROCESS_LIFE */
    { 0u         , 0u     }, /* HK_LIFE */
    { 0u         , 0u     }, /* AOCS_LIFE */
    { 0u         , 0u     }, /* POWER_LIFE */
    { 0u         , 0u     }, /* THERMAL_LIFE */
    { 0u         , 0u     }, /* GRAVIMETRY_LIFE */
    { 0u         , 0u     }, /* IRIDIUM_LIFE */
    { 0u         , 0u     }, /* MISO_MODE_CHANGE */
    { 0u         , 0u     }, /* CARNE_MODE_CHANGE */
    { 0u         , 0u     }, /* SALAMI_EVENTS */
    { 0u         , 0u     }, /* MISO_EVENTS */
    { 0u         , 0u     }, /* AOCS_EVENTS */
    { 0u         , 0u     }, /* POWER_EVENTS */
    { 0u         , 0u     }, /* THERMAL_EVENTS */
    { 0u         , 0u     }, /* GRAVIMETRY_EVENTS */
    { 0u         , 0u     }, /* IRIDIUM_EVENTS */
    { 0u         , 0u     }, /* AOCS_HK */
    { 0u         , 0u     }, /* POWER_HK */
    { 0u         , 0u     }, /* THERMAL_HK */
    { 0u         , 0u     }, /* GRAVIMETRY_HK */
    { 0u         , 0u     }, /* IRIDIUM_HK */
    { 0u         , 0u     }, /* TC_NORMAL */
    { 0u         , 0u     }, /* TC_PUS3 */
    { 0u         , 0u     }, /* TC_PUS5 */
    { 0u         , 0u     }, /* TC_PUS11 */
    { 0u         , 0u     }, /* TC_PUS160 */
    { 0u         , 0u     }, /* TC_PUS161 */
    { 0u         , 0u     }, /* TC_PUS176 */
    { 0u         , 0u     }, /* TC_PUS177 */
    { 0u         , 0u     }, /* TC_PUS178 */
    { 0u         , 0u     }, /* TC_PUS192 */
    { 0u         , 0u     }, /* TC_PUS193 */
    { 0u         , 0u     }, /* TC_DELAYED */
    { 0u         , 0u     }, /* TM_NORMAL */
    { 0u         , 0u     }, /* TM_PUS1 */
    { 0u         , 0u     }, /* TM_PUS3 */
    { 0u         , 0u     }, /* TM_PUS5 */
    { 0u         , 0u     }, /* TM_PUS160 */
    { 0u         , 0u     }, /* TM_PUS161 */
    { 0u         , 0u     }, /* TM_PUS176 */
    { 0u         , 0u     }, /* TM_PUS177 */
    { 0u         , 0u     }, /* TM_PUS178 */
    { 0u         , 0u     }, /* TM_PUS192 */
    { 0u         , 0u     }, /* TM_PUS193 */
};
