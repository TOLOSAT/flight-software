/**
 * @file    pus161.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 161 functions (Test)
 * @date    12/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus17 PUS Service 17
 * Generic functions used by PUS17 (Test)
 * @{
 */

#ifndef PUS161_H
#define PUS161_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS161SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS161SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS161SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS161_H */

/** 
 * @} 
 * @} 
 */