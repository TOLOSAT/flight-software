/**
 * @file    buffers_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration table for buffers
 * @date    26/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_buffers_static_conf
 * @brief   Configuration table where all buffers static parameters are stored
 */
const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS] = 
{
    /* Buffer Ref , Sender Ref  , Receiver Ref , Msg Size , Msg Nb */
    { BUFF01      , MAIN_TASK   , SECOND_TASK  , 12U      , 1U     },
    { BUFF02      , SECOND_TASK , MAIN_TASK    , 12U      , 1U     },
};

/**
 * @var     g_buffers_dynamic_conf
 * @brief   Configuration table where all buffers dynamic parameters are stored
 */
bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS] = 
{
    {.buffer_data = g_buff01_data},
    {.buffer_data = g_buff02_data},
};

/**
 * @var     g_buff01_data
 * @brief   Data array for BUFF01
 */
bufferData_t g_buff01_data[12u*1u] = {0};

/**
 * @var     g_buff02_data
 * @brief   Data array for BUFF02
 */
bufferData_t g_buff02_data[12u*1u] = {0};