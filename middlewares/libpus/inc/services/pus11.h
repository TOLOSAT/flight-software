/**
 * @file    pus11.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 11 functions (Time-based scheduling)
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus11 PUS Service 11
 * Generic functions used by PUS11 (Time-based scheduling)
 * @{
 */

#ifndef PUS11_H
#define PUS11_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    pus11Status_t
 * @brief   PUS 11 status enum
 */
typedef enum
{
    PUS11_DISABLE = 0u,  /**< PUS11 is disabled */
    PUS11_ENABLE = 1u,   /**< PUS11 is enabled */
} pus11Status_t;

/*************************** Variables Declarations **************************/

extern pus11Status_t g_pus11_status;

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
pusStatus_t ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS11_H */

/** 
 * @} 
 * @} 
 */