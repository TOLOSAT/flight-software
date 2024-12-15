/**
 * @file    drv_rtc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for RTC functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_rtc.h"
#include "drv/drv_tim.h"

/***************************** Macros Definitions ****************************/

#define RTC_DEFAULT_YEAR            0u      /**< Default year */
#define RTC_DEFAULT_MONTH           1u      /**< Default month */
#define RTC_DEFAULT_DAY             1u      /**< Default day */

#define DAYS_PER_MONTH              30u     /**< Number of days per month (arbitrary set to 30 because it is just for emulation not real RTC)*/
#define HOURS_PER_DAY               24u     /**< Number of hours per day */
#define MINUTES_PER_HOUR            60u     /**< Number of minutes per hour */
#define SECONDS_PER_DAY             86400u  /**< Number of second per day */
#define SECONDS_PER_HOUR            3600u   /**< Number of second per hour */
#define SECONDS_PER_MINUTE          60u     /**< Number of second per minute */
#define MILLISECONDS_PER_SECOND     1000u   /**< Number of millisecond per second */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitRtc(void)
 * @brief   Function that initialise RTC
 * @retval  #RET_SUCCESSFUL always
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 *
 * This function does nothing because everything is already done by
 * the CMSDK HAL.
 */
returnCode_t InitRtc(void)
{
    return RET_SUCCESSFUL;
}

/**
 * @fn          RtcSetTime(const rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time    Value of RTC time we want to set
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 *
 * The HAL tick cannot be set for compatibility reasons with the ST HAL.
 * So no setup is possible at the moment.
 */
returnCode_t RtcSetTime(const rtcTime_t *rtc_time)
{
    (void)(rtc_time);

    return RET_SUCCESSFUL;
}

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time    Value to RTC time we want to read
 * @retval      #RET_SUCCESSFUL always
 *
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 */
returnCode_t RtcGetTime(rtcTime_t *rtc_time)
{
    uint32_t tick = cmsdk_HalGetTick();
    rtc_time->year = RTC_DEFAULT_YEAR;      // CONSTANT
    rtc_time->month = RTC_DEFAULT_MONTH;    // CONSTANT
    rtc_time->day = ((tick / MILLISECONDS_PER_SECOND / SECONDS_PER_DAY) % 30u) + RTC_DEFAULT_DAY;
    rtc_time->hour = (tick / MILLISECONDS_PER_SECOND / SECONDS_PER_HOUR) % HOURS_PER_DAY;
    rtc_time->minute = (tick/ MILLISECONDS_PER_SECOND / SECONDS_PER_MINUTE) % MINUTES_PER_HOUR;
    rtc_time->second = (tick / MILLISECONDS_PER_SECOND) % SECONDS_PER_MINUTE;
    rtc_time->subsecond = ((tick % MILLISECONDS_PER_SECOND) << 16) / MILLISECONDS_PER_SECOND;
    return RET_SUCCESSFUL;
}