/**
 * @file    hk_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for hk environment
 * @date    09/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/hk_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_hk_conf
 * @brief   Configuration table where all HK dynamic parameters are stored
 * @warning HKID must be ordered from smallest to largest
 * @warning The ref number must correspond to the line number.
 */
pusHkConf_t g_hk_conf[NB_HK] =
{   
    /* HK Ref  , HKID          , HK_STATUS   */
    { DUMMY_HK , 0x00000001u   , PUS3_ENABLE },
};