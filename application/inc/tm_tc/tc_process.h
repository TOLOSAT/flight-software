/**
 * @file    tc_process.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC_PROCESS Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TC_PROCESS_H
#define TC_PROCESS_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_EXECUTION    7u  /**< Number of exution functions */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_tc_execution_table[NB_EXECUTION];

/*************************** Functions Declarations **************************/

void TcProcessMain(void *task_dyn_conf);

#endif /* TC_PROCESS_H */