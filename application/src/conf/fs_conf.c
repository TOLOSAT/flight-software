/**
 * @file    fs_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for file system content
 * @date    09/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/fs_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_TMPFS_SECTION    __attribute__((section(".tmpfs")))  /**< Temporary file goes to .tmpfs section */

/*************************** Variables Definitions ***************************/

/**
 * @var     g_files_conf
 * @brief   Configuration table where all files static parameters are stored
 */
const fsFileConf_t g_files_conf[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES] = 
{
        /* Fileno          , File Name      , File Access Mode                    , Temp File              */
    {
        { TEST_FILE        , "test.txt"     , FA_OPEN_ALWAYS | FA_WRITE | FA_READ , &test_temp_file        },
        { CONSOLE_FILE     , "console.txt"  , FA_OPEN_ALWAYS | FA_WRITE | FA_READ , &console_temp_file     },
        { PUS11_SCHED_FILE , "pus11sch.bin" , FA_OPEN_ALWAYS | FA_WRITE | FA_READ , &pus11_sched_temp_file },
        { PUS11_DATA_FILE  , "pus11dat.bin" , FA_OPEN_ALWAYS | FA_WRITE | FA_READ , &pus11_data_temp_file  },
    }
};

/**
 * @var     test_temp_file
 * @brief   Temporary file used for TEST_FILE
 */
FIL IN_TMPFS_SECTION test_temp_file = {0};

/**
 * @var     console_temp_file
 * @brief   Temporary file used for CONSOLE_FILE
 */
FIL IN_TMPFS_SECTION console_temp_file = {0};

/**
 * @var     pus11_sched_temp_file
 * @brief   Temporary file used for PUS11_SCHED_FILE
 */
FIL IN_TMPFS_SECTION pus11_sched_temp_file = {0};

/**
 * @var     pus11_data_temp_file
 * @brief   Temporary file used for PUS11_DATA_FILE
 */
FIL IN_TMPFS_SECTION pus11_data_temp_file = {0};