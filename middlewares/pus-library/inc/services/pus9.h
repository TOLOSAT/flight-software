/**
 * @file    pus9.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 9 functions (Time Management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus9 PUS Service 9
 * @brief PUS service 9 (Time Management) implementation
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

extern returnCode_t ExecuteS9SS128(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS9_H */

/**
 * @}
 * @}
 * @}
 */