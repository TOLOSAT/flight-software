/**
 * @file    fs_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration for file system content
 * @date    09/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "conf/fs_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_files_conf
 * @brief   Configuration table where all files static parameters are stored
 */
const fsFileConf_t g_files_conf[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES] = 
{
        /* Fileno          , File Name      , File Access Mode                    */
    {
        { TEST_FILE        , "test.txt"     , FA_OPEN_ALWAYS | FA_WRITE | FA_READ },
        { PUS11_SCHED_FILE , "pus11sch.txt" , FA_OPEN_ALWAYS | FA_WRITE | FA_READ },
        { PUS11_DATA_FILE  , "pus11dat.txt" , FA_OPEN_ALWAYS | FA_WRITE | FA_READ },
    }
};