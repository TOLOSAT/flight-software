/**
 * @file time_management.c
 * @author Merlin Kooshmanian
 * @brief Source file for Time Management
 * @date 23/07/2023
 *
 * Last Update : 23/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <time.h> // cppcheck-suppress misra-c2012-21.10

#include "pus_tools/time_management.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void ConvertRTCtoReadableTime(const rtcTime_t *rtc_time, struct tm *readable_time);
static void ConvertReadableTimetoRTC(rtcTime_t *rtc_time, const struct tm *readable_time);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          GetCUCTime(cucTime_t *cuc_time)
 * @brief       Function that gets CUC time from RTC
 * @param[out]  cuc_time time formated according to CUC
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot get RTC time
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t GetCUCTime(cucTime_t *cuc_time)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    rtcTime_t rtc_time = {0};
    struct tm readable_time = {0};

    // Function Core
    if (cuc_time != NULL)
    {
        // Get Time from RTC
        halStatus_t test_val = RtcGetTime(&rtc_time);
        if (test_val == FCT_SUCCESSFUL)
        {
            // Convert RTC to UNIX Time
            ConvertRTCtoReadableTime(&rtc_time, &readable_time);
            time_t unix_time = mktime(&readable_time);

            // Convert UNIX Time to CUC Time
            uint32_t raw_cuc_time = (uint32_t)(unix_time) + TAI_UNIX_OFFSET;
            cuc_time->time_header = TIME_HEAD_CONSTANT;
            cuc_time->coarse_time[0] = (uint8_t)((0xff000000u & raw_cuc_time) >> 24u);
            cuc_time->coarse_time[1] = (uint8_t)((0x00ff0000u & raw_cuc_time) >> 16u);
            cuc_time->coarse_time[2] = (uint8_t)((0x0000ff00u & raw_cuc_time) >> 8u);
            cuc_time->coarse_time[3] = (uint8_t)(0x000000ffu & raw_cuc_time);
            cuc_time->fine_time[0] = rtc_time.millisecond * 256u / 1000u;
        }
        else
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetCUCTime(cucTime_t *cuc_time)
 * @brief       Function that sets RTC from CUC time
 * @param[out]  cuc_time time formated according to CUC
 * @retval      #PUS_INVALID_PARAM if a pointer is NULL
 * @retval      #PUS_ERROR if cannot set RTC time
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t SetCUCTime(cucTime_t *cuc_time)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    rtcTime_t rtc_time = {0};
    struct tm readable_time;

    // Function Core
    if (cuc_time != NULL)
    {
        // Get UNIX Time from CUC Time
        time_t unix_time = ((time_t)(cuc_time->coarse_time[3]) << 24u) +
                           ((time_t)(cuc_time->coarse_time[2]) << 16u) +
                           ((time_t)(cuc_time->coarse_time[1]) << 8u) +
                           ((time_t)(cuc_time->coarse_time[0])) - TAI_UNIX_OFFSET;
        // Convert UNIX Time to readable time
        readable_time = *localtime(&unix_time);
        // Convert readable time to RTC time
        ConvertReadableTimetoRTC(&rtc_time, &readable_time);
        // Set Time from RTC
        halStatus_t test_val = RtcSetTime(&rtc_time);
        if (test_val != FCT_SUCCESSFUL)
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertRTCtoReadableTime(rtcTime_t *rtc_time, struct tm *readable_time)
 * @brief       Function that convert rtc time into readable time (from libc)
 * @param[in]   rtc_time
 * @param[out]  readable_time
 */
static void ConvertRTCtoReadableTime(const rtcTime_t *rtc_time, struct tm *readable_time)
{
    readable_time->tm_year = (int)(rtc_time->year) + 2000 - 1900;
    readable_time->tm_mon = (int)(rtc_time->month) - 1;
    readable_time->tm_mday = (int)(rtc_time->day);
    readable_time->tm_hour = (int)(rtc_time->hour);
    readable_time->tm_min = (int)(rtc_time->minute);
    readable_time->tm_sec = (int)(rtc_time->second);
}

/**
 * @fn          ConvertReadableTimetoRTC(rtcTime_t *rtc_time, struct tm *readable_time)
 * @brief       Function that convert readable time (from libc) into rtc time
 * @param[in]   readable_time
 * @param[out]  rtc_time
 */
static void ConvertReadableTimetoRTC(rtcTime_t *rtc_time, const struct tm *readable_time)
{
    rtc_time->year = (uint8_t)(readable_time->tm_year + 1900 - 2000);
    rtc_time->month = (uint8_t)(readable_time->tm_mon + 1);
    rtc_time->day = (uint8_t)(readable_time->tm_mday);
    rtc_time->hour = (uint8_t)(readable_time->tm_hour);
    rtc_time->minute = (uint8_t)(readable_time->tm_min);
    rtc_time->second = (uint8_t)(readable_time->tm_sec);
}
