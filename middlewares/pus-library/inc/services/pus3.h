/**
 * @file    pus3.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 3 functions (Housekeeping)
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#define HOUSEKEEPING_ID_SIZE     4u                                              /**< HouseKeeping ID size */
#define HOUSEKEEPING_DATA_SIZE   10u                                             /**< HouseKeeping data size */
#define HOUSEKEEPING_REPORT_SIZE (HOUSEKEEPING_ID_SIZE + HOUSEKEEPING_DATA_SIZE) /**< HouseKeeping report size */

/***************************** Types Definitions *****************************/

/**
 * @struct  housekeepingReport_t
 * @brief   Struct type for an housekeeping report
 */
typedef struct
{
    uint32_t HKID;                        /**< @brief HouseKeeping ID */
    uint8_t data[HOUSEKEEPING_DATA_SIZE]; /**< @brief HouseKeeping data */
} ATTR_BYTE_ALIGNED housekeepingReport_t;
ASSERT_SIZE(housekeepingReport_t, HOUSEKEEPING_REPORT_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t BuildS3SS25(pusTM_t *tm, housekeepingReport_t *report);
extern returnCode_t ExecuteS3SS5(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS3SS6(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS3_H */

/**
 * @}
 * @}
 * @}
 */