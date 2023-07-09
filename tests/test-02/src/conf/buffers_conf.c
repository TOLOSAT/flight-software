/**
 * @file buffers_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking configuration table for buffers
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Configuration *********************************/

/**
 * @var     g_buffers_static_conf
 * @brief   Configuration table where all buffers static parameters are stored
 */
const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS] = 
{
    /* Buffer Ref      , Sender Ref      , Receiver Ref    , Msg Size , Msg Nb */
    {TC_NORMAL         , TC_RECEIVER     , TC_PROCESS      , 256U     , 4U     },
    {TC_PUS11          , TC_RECEIVER     , TC_SCHEDULER    , 256U     , 4U     },
    {TC_DELAYED        , TC_SCHEDULER    , TC_RECEIVER     , 256U     , 4U     },
    {TM_NORMAL         , TC_PROCESS      , TM_SENDER       , 256U     , 4U     },
    {TM_PUS1           , TC_RECEIVER     , TM_SENDER       , 256U     , 4U     },
};

/**
 * @var     g_buffers_dynamic_conf
 * @brief   Configuration table where all buffers dynamic parameters are stored
 */
bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS] = 
{
    /* Buffer ID , Nb Msg */
    { 0u         , 0u     }, /* TC_NORMAL */
    { 0u         , 0u     }, /* TC_PUS11 */
    { 0u         , 0u     }, /* TC_DELAYED */
    { 0u         , 0u     }, /* TM_NORMAL */
    { 0u         , 0u     }, /* TM_PUS1 */
};
