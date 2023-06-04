/**
 * @file buffers_conf.c
 * @author Merlin Kooshmanian
 * @brief Source file stocking configuration table for buffers
 * @date 26/04/2023
 *
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Configuration *********************************/

/**
 * @var     g_buffers_conf
 * @brief   Configuration table that defining all buffers
 */
const bufferDef_t g_buffers_conf[NB_BUFFERS] = 
{
    /* Buffer Ref  , Sender Ref   , Receiver Ref , Msg Size , Msg Nb */
    {BUFF01_BUFFER , MAIN_TASK    , MAIN_TASK    , 10U      , 1U     },
};

/**
 * @var     g_buffers_ids
 * @brief   Table that will stock all buffer ids
 */
bufferId_t g_buffers_ids[NB_BUFFERS];