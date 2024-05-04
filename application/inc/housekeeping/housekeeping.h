/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for HK_MGMT Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef HK_MGMT_H
#define HK_MGMT_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS3_EXECUTION    2u  /**< Number of pus3 exution functions */

#define IN_HK_TEXT_SECTION    __attribute__((section(".text_hk")))  /**< HK functions goes in the .text_hk */
#define IN_HK_DATA_SECTION    __attribute__((section(".data_hk")))  /**< HK data goes in the .data_hk */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_pus3_execution_table[NB_PUS3_EXECUTION];

/*************************** Functions Declarations **************************/

void HkMain(void *task_dyn_conf);

#endif /* HK_MGMT_H */