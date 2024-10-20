/**
 * @file    pus6.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 6 functions (Memory Management)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus6 PUS Service 6
 * @brief PUS service 6 (Memory Management) implementation
 * @{
 */

#ifndef PUS6_H
#define PUS6_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS6_H */

/** 
 * @}
 * @}
 * @}
 */
