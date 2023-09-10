/**
 * @file    pus6_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for pus6 environment
 * @date    09/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/pus6_conf.h"
#include "tolosat_fs_types.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_files_static_conf
 * @brief   Configuration table where all files static parameters are stored
 */
const pus6StaticConf_t g_files_static_conf[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES] = 
{
        /* File Ref, File Name , File Access Mode                    */
    {
        { TEST_FILE, "test.txt" , FA_OPEN_ALWAYS | FA_WRITE | FA_READ },
    }
};