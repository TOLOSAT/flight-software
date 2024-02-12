/**
 * @file    fs_conf.h
 * @author  Merlin Kooshmanian
 * @brief   Header file stocking configuration for file system content
 * @date    09/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef FS_CONF_H
#define FS_CONF_H

/******************************* Include Files *******************************/

#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

#define MAX_NB_FILES_PER_DEVICES    (NB_SD0_FILES)  /**< Maximum Number of files */

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
    PUS11_SCHED_FILE,
    PUS11_DATA_FILE,
    NB_SD0_FILES,
};

/*************************** Variables Declarations **************************/

extern const fsFileConf_t g_files_conf[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES];

#endif /* FS_CONF_H */