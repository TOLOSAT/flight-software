/**
 * @file    tc_execution.h
 * @author  Merlin Kooshmanian
 * @brief   Source file including function for generic TC execution
 * @date    14/09/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup tc_execution TC Execution
 * Function for generic TC execution
 * @{
 */

#ifndef TC_EXECUTION_H
#define TC_EXECUTION_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "buffers.h"
#include "pus_tools/tc_management.h"
#include "pus_tools/tm_management.h"
#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void ExecuteTC(pusExecutionTable_t *execution_table, pusTableSize_t table_size, bufferRef_t input_buffer, bufferRef_t output_buffer);
void SendExecAckTM(pusTC_t *tc, pusTM_t *execution_tm);
void SendExecNackTM(pusTC_t *tc, pusTM_t *execution_tm, pusExecutionError_t execution_error);

#endif /* TC_EXECUTION_H */

/** 
 * @} 
 * @} 
 */