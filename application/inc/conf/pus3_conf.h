/**
 * @file    pus3_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration for pus3 environment
 * @date    10/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS3_CONF_H
#define PUS3_CONF_H

/******************************* Include Files *******************************/

#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    HK_ENUM
 * @brief   Enum defining hk reference numbers
 */
enum HK_ENUM 
{
    DUMMY_HK = 0u,
    NB_HK,
};

/*************************** Variables Declarations **************************/

extern pus3DynamicConf_t g_hk_dynamic_conf[NB_HK];

#endif /* PUS3_CONF_H */