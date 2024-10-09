/**
 * @file    pus3.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 3 functions (Housekeeping)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus3 PUS Service 3
 * @brief PUS service 3 (Housekeeping) implementation
 * @{
 */

#ifndef PUS3_H
#define PUS3_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t BuildS3SS25(pusTM_t *tm, housekeepingReport_t *report);
extern returnCode_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t IsHKReportAvailable(hkId_t hkid);

#endif /* PUS3_H */

/** 
 * @}
 * @}
 * @}
 */