/**
 * @file    pus9.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 9 functions (Time management)
 * @date    29/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus9 PUS Service 9
 * Generic functions used by PUS9 (Time management)
 * @{
 */

#ifndef PUS9_H
#define PUS9_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS9SS128(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS1_H */

/** 
 * @} 
 * @} 
 */