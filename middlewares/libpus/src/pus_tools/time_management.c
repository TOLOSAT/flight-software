/**
 * @file    time_management.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <time.h> // cppcheck-suppress misra-c2012-21.10

#include "pus_tools/time_management.h"
#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define SECONDS_IN_DAY 86400u         /**< Number of seconds in a day */
#define SECONDS_IN_HOUR 3600u         /**< Number of seconds in a hour */
#define SECONDS_IN_MINUTE 60u         /**< Number of seconds in a minute */
#define DAYS_IN_YEAR 365u             /**< Number of days in a year */
#define DAYS_IN_LEAP_YEAR 366u        /**< Number of days in a leap year (occures every 4 years execept some years) */
#define JANUARY_FIRST_2000 946684800u /**< UNIX timestamp for january 1rst 2000 (TOLOSAT RTC cannot compute time before this date) */

/*************************** Functions Declarations **************************/

static void ConvertRTCtoReadableTime(const rtcTime_t *rtc_time, struct tm *readable_time);
static pusStatus_t ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time);

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

    // Function Core
    if (cuc_time != NULL)
    {
        // Get UNIX Time from CUC Time
        time_t unix_time = ((time_t)(cuc_time->coarse_time[3]) << 24u) +
                           ((time_t)(cuc_time->coarse_time[2]) << 16u) +
                           ((time_t)(cuc_time->coarse_time[1]) << 8u) +
                           ((time_t)(cuc_time->coarse_time[0])) - TAI_UNIX_OFFSET;
        // Convert UNIX Time to RTC Time
        return_value = ConvertUnixTimestampToRTCTime(unix_time, &rtc_time);
        if (return_value == PUS_SUCCESSFUL)
        {
            // Set Time from RTC
            halStatus_t test_val = RtcSetTime(&rtc_time);
            if (test_val != FCT_SUCCESSFUL)
            {
                return_value = PUS_ERROR;
            }
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
 * @fn          ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
 * @brief       Function that convert Unix timestamp into RTC time
 * @param[in]   unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @param[out]  rtc_time RTC time (as it has been defined in TOLOSAT HAL)
 * @retval      #PUS_INVALID_PARAM if a rtc_time is NULL or timestamp is before january 1rst 2000
 * @retval      #PUS_ERROR if RTC time has not been computed correctly
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    uint32_t timestamp = unix_timestamp;

    // Function Core
    if ((rtc_time != NULL) || (timestamp < JANUARY_FIRST_2000))
    {
        uint32_t year = 1970u;

        // Year Calculation
        while (timestamp >= (SECONDS_IN_DAY * DAYS_IN_YEAR))
        {
            if ((((year % 4u) == 0u) && ((year % 100u) != 0u)) || ((year % 400u) == 0u))
            {
                timestamp -= (SECONDS_IN_DAY * DAYS_IN_LEAP_YEAR);
            }
            else
            {
                timestamp -= (SECONDS_IN_DAY * DAYS_IN_YEAR);
            }
            year++;
        }

        // Check if year is superior to 2000 because TOLOSAT RTC cannot support date before January 1rst 2000
        if (year >= 2000u)
        {
            // Set RTC Time year field
            rtc_time->year = (uint8_t)(year - 2000u);

            // Month and Month Day Calculation
            const uint8_t daysInMonth[12] = {31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};
            rtc_time->month = 1u;
            while ((rtc_time->month <= 12u) && (timestamp >= (SECONDS_IN_DAY * daysInMonth[rtc_time->month - 1u])))
            {
                uint8_t daysInCurrentMonth = daysInMonth[rtc_time->month - 1u];
                if ((rtc_time->month == 2u) && ((((year % 4u) == 0u) && ((year % 100u) != 0u)) || ((year % 400u) == 0u)))
                {
                    daysInCurrentMonth = 29u;
                }
                timestamp -= (SECONDS_IN_DAY * daysInCurrentMonth);
                rtc_time->month++;
            }
            rtc_time->day = (timestamp / SECONDS_IN_DAY) + 1u;
            timestamp %= SECONDS_IN_DAY;

            // Hour, Minute and Second Calculation
            rtc_time->hour = timestamp / SECONDS_IN_HOUR;
            timestamp %= SECONDS_IN_HOUR;
            rtc_time->minute = timestamp / SECONDS_IN_MINUTE;
            rtc_time->second = timestamp % SECONDS_IN_MINUTE;
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