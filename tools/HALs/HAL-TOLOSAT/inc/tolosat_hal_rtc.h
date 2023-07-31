/**
 * @file    tolosat_hal_rtc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL RTC functions
 * @date    18/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_rtc Driver RTC
 * Functions that allows to drive Real Time Clock
 * @{
 */

#ifndef TOLOSAT_HAL_RTC_H
#define TOLOSAT_HAL_RTC_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief RTC handle struct type redefinition */
typedef RTC_HandleTypeDef rtcHandleStruct_t;

/** 
 * @struct  rtcTime_t
 * @brief   Struct type definition for Time storage struct
 */
typedef struct
{
    uint8_t year;           /**< RTC year */
    uint8_t month;          /**< RTC month */
    uint8_t day;            /**< RTC day */
    uint8_t hour;           /**< RTC hour */
    uint8_t minute;         /**< RTC minute */
    uint8_t second;         /**< RTC second */
    uint32_t millisecond;   /**< RTC millisecond */
} rtcTime_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t RtcInit(void);
halStatus_t RtcSetTime(rtcTime_t *rtc_time);
halStatus_t RtcGetTime(rtcTime_t *rtc_time);


#endif /* TOLOSAT_HAL_RTC_H */

/** 
 * @} 
 * @} 
 */