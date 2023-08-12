/**
 * @file    time_management.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

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

static pusStatus_t ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp);
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

    // Function Core
    if (cuc_time != NULL)
    {
        // Get Time from RTC
        halStatus_t test_val = RtcGetTime(&rtc_time);
        if (test_val == FCT_SUCCESSFUL)
        {
            // Convert RTC to RAW CUC Time (TAI)
            uint32_t raw_cuc_time;
            return_value = ConvertRTCTimeToUnixTimestamp(rtc_time, &raw_cuc_time);
            if (return_value == PUS_SUCCESSFUL)
            {
                // Add TAI offset
                raw_cuc_time += TAI_UNIX_OFFSET;

                // Convert RAW CUC Time to CUC Time
                cuc_time->time_header = TIME_HEAD_CONSTANT;
                cuc_time->coarse_time[0] = (uint8_t)((0xff000000u & raw_cuc_time) >> 24u);
                cuc_time->coarse_time[1] = (uint8_t)((0x00ff0000u & raw_cuc_time) >> 16u);
                cuc_time->coarse_time[2] = (uint8_t)((0x0000ff00u & raw_cuc_time) >> 8u);
                cuc_time->coarse_time[3] = (uint8_t)(0x000000ffu & raw_cuc_time);
                cuc_time->fine_time[0] = rtc_time.millisecond * 256u / 1000u;
            }
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
        uint32_t unix_time = ((uint32_t)(cuc_time->coarse_time[3]) << 24u) +
                             ((uint32_t)(cuc_time->coarse_time[2]) << 16u) +
                             ((uint32_t)(cuc_time->coarse_time[1]) << 8u) +
                             ((uint32_t)(cuc_time->coarse_time[0])) - TAI_UNIX_OFFSET;
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
 * @fn          ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
 * @brief       Function that convert RTC time into Unix timestamp
 * @param[in]   rtc_time RTC time (as it has been defined in TOLOSAT HAL)
 * @param[out]  unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @retval      #PUS_INVALID_PARAM if a timestamp is NULL
 * @retval      #PUS_SUCCESSFUL else
 */
static pusStatus_t ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if (unix_timestamp != NULL)
    {
        // Numbers of day each month
        const uint8_t DAYS_IN_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

        // Compute total of days elapsed since january 1rst 1970
        uint32_t total_days = (rtc_time.year + 30u) * DAYS_IN_YEAR;
        for (uint16_t i = 1970u; i < ((uint16_t)rtc_time.year + 2000u); i++)
        {
            if ((((i % 4u) == 0u) && ((i % 100u) != 0u)) || ((i % 400u) == 0u))
            {
                total_days++; // Add 1 day for leap years
            }
        }

        for (uint8_t i = 0u; i < (rtc_time.month - 1u); i++)
        {
            total_days += DAYS_IN_MONTH[i];
            if ((i == 1u) && ((((rtc_time.year % 4u) == 0u) && ((rtc_time.year % 100u) != 0u)) || ((rtc_time.year % 400u) == 0u)))
            {
                total_days++; // Add 1 day for february month during leap year
            }
        }
        total_days += rtc_time.day - 1u;

        // Total seconds elapsed computation
        uint32_t total_seconds = (total_days * SECONDS_IN_DAY) + (rtc_time.hour * SECONDS_IN_HOUR) + (rtc_time.minute * SECONDS_IN_MINUTE) + rtc_time.second;

        // Update unix_timestamp with total_second
        *unix_timestamp = total_seconds;
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
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
            const uint8_t days_in_month[12] = {31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u};
            rtc_time->month = 1u;
            while ((rtc_time->month <= 12u) && (timestamp >= (SECONDS_IN_DAY * days_in_month[rtc_time->month - 1u])))
            {
                uint8_t days_in_current_month = days_in_month[rtc_time->month - 1u];
                if ((rtc_time->month == 2u) && ((((year % 4u) == 0u) && ((year % 100u) != 0u)) || ((year % 400u) == 0u)))
                {
                    days_in_current_month = 29u;
                }
                timestamp -= (SECONDS_IN_DAY * days_in_current_month);
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