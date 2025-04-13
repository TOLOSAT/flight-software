/**
 * @file    pus160.h
 * @author  Théo Bessel
 * @brief   Header file for PUS 160 functions (System Management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus160 PUS Service 160
 * @brief PUS service 160 (System Management) implementation
 * @{
 */

#ifndef PUS160_H
#define PUS160_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS160_H */

/**
 * @}
 * @}
 * @}
 */
