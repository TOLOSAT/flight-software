/**
 * @file    tc_scheduler.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TC_SCHEDULER Task
 * @date    02/07/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TC_SCHEDULER_H
#define TC_SCHEDULER_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

#define NB_PUS11_EXECUTION    4u  /**< Number of pus11 exution functions */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern pusExecutionTable_t g_pus11_execution_table[NB_PUS11_EXECUTION];

/*************************** Functions Declarations **************************/

void TcSchedulerMain(void *task_dyn_conf);

#endif /* TC_SCHEDULER_H */