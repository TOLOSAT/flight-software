/**
 * @file    pus6_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration for pus6 environment
 * @date    09/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef PUS6_CONF_H
#define PUS6_CONF_H

/******************************* Include Files *******************************/

#include "services/pus6.h"

/***************************** Macros Definitions ****************************/

#define MAX_NB_FILES_PER_DEVICES    1u  /**< Maximum Number of files */

/***************************** Types Definitions *****************************/

/**
 * @enum    MEMORY_DEVICES_ENUM
 * @brief   Enum defining memory devices reference numbers
 */
enum MEMORY_DEVICES_ENUM 
{
    SD0 = 0u,
    NB_MEMORY_DEVICES,
};

/**
 * @enum    FILE_SD0_ENUM
 * @brief   Enum defining file in SD0 reference numbers
 */
enum FILE_SD0_ENUM 
{
    TEST_FILE = 0u,
    NB_SD0_FILES,
};

/*************************** Variables Declarations **************************/

extern const pus6StaticConf_t g_files_static_conf[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES];

#endif /* PUS6_CONF_H */