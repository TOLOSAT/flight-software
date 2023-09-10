/**
 * @file    pus3_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for pus3 environment
 * @date    09/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/pus3_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_hk_dynamic_conf
 * @brief   Configuration table where all HK dynamic parameters are stored
 */
pus3DynamicConf_t g_hk_dynamic_conf[NB_HK] =
{   
    /* HK Ref , HKID , HK_STATUS */
    { DUMMY_HK, 0u   , PUS3_ENABLE},
};