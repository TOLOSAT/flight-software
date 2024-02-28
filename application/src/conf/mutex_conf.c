/**
 * @file    mutex_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration table for mutex
 * @date    28/10/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/mutex_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutex_conf
 * @brief   Configuration table where all mutexes parameters are stored
 */
mutexConf_t g_mutex_conf[NB_MUTEXES] = 
{
    /* Mutex Ref , Mutex Handler */
    { DUMMY_MUTEX   , 0u            },
};