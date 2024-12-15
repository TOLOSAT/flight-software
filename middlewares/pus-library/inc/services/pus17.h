/**
 * @file    pus17.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 17 functions (Test)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus17 PUS Service 17
 * @brief PUS service 17 (Test) implementation
 * @{
 */

#ifndef PUS17_H
#define PUS17_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS17_H */

/**
 * @}
 * @}
 * @}
 */