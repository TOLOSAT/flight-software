/**
 * @file    pus17.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 17 functions (Test)
 * @date    12/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus17 PUS Service 17
 * Generic functions used by PUS17 (Test)
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

extern pusStatus_t ExecuteS17SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS17_H */

/** 
 * @} 
 * @} 
 */