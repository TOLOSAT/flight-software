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
 * @warning HKID must be ordered from smallest to largest
 * @warning The ref number must correspond to the line number.
 */
pus3DynamicConf_t g_hk_dynamic_conf[NB_HK] =
{   
    /* HK Ref  , HKID          , HK_STATUS   */
    { DUMMY_HK , 0x00000001u   , PUS3_ENABLE },
};