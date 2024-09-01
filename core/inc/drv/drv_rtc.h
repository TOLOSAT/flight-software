/**
 * @file    drv_rtc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RTC functions
 * @date    18/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_rtc Driver RTC
 * Functions that allows to drive Real Time Clock
 * @{
 */

#ifndef DRV_RTC_H
#define DRV_RTC_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

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
    uint16_t subsecond;     /**< RTC 16 bits subsecond fraction (0x0001 = 2^(-16) s) */
} rtcTime_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t InitRtc(void);
extern coreStatus_t RtcSetTime(const rtcTime_t *rtc_time);
extern coreStatus_t RtcGetTime(rtcTime_t *rtc_time);


#endif /* DRV_RTC_H */

/** 
 * @} 
 * @} 
 */