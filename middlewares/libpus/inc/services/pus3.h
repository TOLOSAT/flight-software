/**
 * @file    pus3.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 3 functions (Housekeeping)
 * @date    06/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus3 PUS Service 3
 * Generic functions used by PUS3 (Housekeeping)
 * @{
 */

#ifndef PUS3_H
#define PUS3_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief HK reference number type */
typedef uint32_t hkRef_t;

/** @brief HK reference number type */
typedef uint32_t hkId_t;

/**
 * @enum    hkStatus_t
 * @brief   PUS 3 HK status enum
 */
typedef enum
{
    PUS3_DISABLE = 0u,  /**< HK is disabled */
    PUS3_ENABLE = 1u,   /**< HK is enabled */
} hkStatus_t;

/** 
 * @struct  pusHkConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{                    
    hkRef_t ref;            /**< @brief HK reference number as it is declared in HK_ENUM */
    hkId_t hkid;            /**< @brief HK ID */
    hkStatus_t hk_status;   /**< @brief HK Status (enable/disable) */
} pusHkConf_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t BuildS3SS25(pusTM_t *tm, housekeepingReport_t *report);
extern pusStatus_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t IsHKReportAvailable(hkId_t hkid);

#endif /* PUS3_H */

/** 
 * @} 
 * @} 
 */