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

#define RTC_DEFAULT_YEAR            0u      /**< Default year alias 2000 */
#define RTC_DEFAULT_MONTH           1u      /**< Default month alias january */
#define RTC_DEFAULT_DAY             1u      /**< Default day alias 1rst */
#define RTC_DEFAULT_HOUR            0u      /**< Default hour alias 0 */
#define RTC_DEFAULT_MINUTE          0u      /**< Default minute alias 0 */
#define RTC_DEFAULT_SECOND          0u      /**< Default second alias 0 */
#define RTC_DEFAULT_MILLISECOND     0u      /**< Default second alias 0 */

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
 * @fn      RtcInit(void)
 * @brief   Function that initialise RTC
 * @retval  #GEN_HAL_SUCCESSFUL always
 * 
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 * 
 * This function does nothing because everything is already done by
 * the CMSDK HAL.
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
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 * 
 * The HAL tick cannot be set for compatibility reasons with the ST HAL.
 * So no setup is possible at the moment.
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
 * @warning     RTC is not supported by CMSDK so it is emulated by HAL tick
 */
halStatus_t IN_RTC_TEXT_SECTION RtcGetTime(rtcTime_t *rtc_time)
{
    uint32_t tick = HalGetTick();
    rtc_time->year = RTC_DEFAULT_YEAR;      // CONSTANT
    rtc_time->month = RTC_DEFAULT_MONTH;    // CONSTANT
    rtc_time->day = (tick / MILLISECONDS_PER_SECOND / SECONDS_PER_DAY) % 30u;
    rtc_time->hour = (tick / MILLISECONDS_PER_SECOND / SECONDS_PER_HOUR) % HOURS_PER_DAY;
    rtc_time->minute = (tick/ MILLISECONDS_PER_SECOND / SECONDS_PER_MINUTE) % MINUTES_PER_HOUR;
    rtc_time->second = (tick / MILLISECONDS_PER_SECOND) % SECONDS_PER_MINUTE;
    rtc_time->millisecond = tick % MILLISECONDS_PER_SECOND;
    return GEN_HAL_SUCCESSFUL;
}