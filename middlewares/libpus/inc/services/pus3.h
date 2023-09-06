/**
 * @file    pus3.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 3 functions
 * @date    06/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus3 PUS Service 3
 * Generic functions used by PUS3
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

pusStatus_t BuildS3SS25(pusTM_t *tm, houseKeepingReport_t *report);
pusStatus_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm);
pusStatus_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm);
pusStatus_t IsHKReportAvailable(uint32_t HKID);

#endif /* PUS3_H */

/** 
 * @} 
 * @} 
 */