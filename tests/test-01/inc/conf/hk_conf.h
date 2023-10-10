/**
 * @file    hk_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration for hk environment
 * @date    10/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef HK_CONF_H
#define HK_CONF_H

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

extern pusHkConf_t g_hk_conf[NB_HK];

#endif /* HK_CONF_H */