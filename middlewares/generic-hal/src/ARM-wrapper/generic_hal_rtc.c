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
    (void)(rtc_time);
    return GEN_HAL_SUCCESSFUL;
}