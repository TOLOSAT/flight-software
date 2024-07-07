/**
 * @file    pus161.h
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Header file for PUS 161 functions (MISO)
 * @date    08/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef PUS161_H
#define PUS161_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @struct  pus161Data_t
 * @brief   Struct type definition of PUS161 data
 */
typedef struct 
{
    uint32_t idle_time;
    uint32_t max_stack_usage;
} pus161Data_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t InitS161(pus161Data_t *pus161_data);
pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS161_H */
