/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for HK_MGMT Task
 * @date    28/06/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef HK_MGMT_H
#define HK_MGMT_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS3_EXECUTION    2u  /**< Number of pus3 exution functions */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_pus3_execution_table[NB_PUS3_EXECUTION];

/*************************** Functions Declarations **************************/

void HkMain(void *task_dyn_conf);

#endif /* HK_MGMT_H */