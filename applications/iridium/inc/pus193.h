/**
 * @file    pus161.h
 * @author  Clement Cognard & Merlin Kooshmanian
 * @brief   Header file for PUS 161 functions (MISO)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus161 PUS Service 161
 * @brief PUS service 161 (Internal Software Monitoring) implementation
 * @{
 */

#ifndef PUS193_H
#define PUS193_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ExecuteS193SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS193SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS193_H */

/**
 * @}
 * @}
 * @}
 */