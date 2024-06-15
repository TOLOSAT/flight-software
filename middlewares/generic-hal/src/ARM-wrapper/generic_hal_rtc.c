/**
 * @file    generic_hal_rtc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL RTC functions
 * @date    18/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define RTC_DEFAULT_YEAR        0u      /**< Default year alias 2000 */
#define RTC_DEFAULT_MONTH       1u      /**< Default month alias january */
#define RTC_DEFAULT_DAY         1u      /**< Default day alias 1rst */
#define RTC_DEFAULT_HOUR        0u      /**< Default hour alias 0 */
#define RTC_DEFAULT_MINUTE      0u      /**< Default minute alias 0 */
#define RTC_DEFAULT_SECOND      0u      /**< Default second alias 0 */
#define RTC_DEFAULT_MILLISECOND 0u      /**< Default second alias 0 */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      RtcInit(void)
 * @brief   Function that initialise RTC
 * @retval  #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     RTC is not supported by CMSDK
 */
halStatus_t IN_RTC_TEXT_SECTION RtcInit(void)
{
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          RtcSetTime(const rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time Value of RTC time we want to set
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     RTC is not supported by CMSDK
 */
halStatus_t IN_RTC_TEXT_SECTION RtcSetTime(const rtcTime_t *rtc_time)
{
    (void)(rtc_time);
    return GEN_HAL_SUCCESSFUL;
}

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time Value to RTC time we want to read
 * @retval      #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     RTC is not supported by CMSDK
 */
halStatus_t IN_RTC_TEXT_SECTION RtcGetTime(rtcTime_t *rtc_time)
{
    uint32_t tick_sec = HalGetTick()/1000u;
    rtc_time->year = RTC_DEFAULT_YEAR;
    rtc_time->month = RTC_DEFAULT_MONTH;
    rtc_time->day = RTC_DEFAULT_DAY;
    rtc_time->hour = (tick_sec/3600u)%24;
    rtc_time->minute = (tick_sec/60u)%60;
    rtc_time->second = tick_sec%60u;
    rtc_time->millisecond = RTC_DEFAULT_MILLISECOND;
    return GEN_HAL_SUCCESSFUL;
}