/**
 * @file    time_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libtime TIME Library
 * @{
 * @defgroup time_management Time Management
 * Functions that allows to format time according to CUC
 * @{
 */

#ifndef TIME_MANAGEMENT_H
#define TIME_MANAGEMENT_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "time_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    timeStatus_t
 * @brief   Time functions specific returns 
 */
typedef enum
{
    TIME_SUCCESSFUL            = 0u,    /**< Function succeed */
    TIME_ERROR                 = 1u,    /**< Function failed */
    TIME_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    TIME_TIMEOUT               = 3u,    /**< Function returned a timeout */
} timeStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

timeStatus_t GetStrCUCTime(char cuc_time_str[CUC_TIME_STR_SIZE]);
timeStatus_t GetCUCTime(cucTime_t *cuc_time);
timeStatus_t SetCUCTime(cucTime_t *cuc_time);
timeStatus_t CompareCUCTimes(cucTime_t *older_cuc_time, cucTime_t *newer_cuc_time);

#endif /* TIME_MANAGEMENT_H */

/** 
 * @} 
 * @} 
 */