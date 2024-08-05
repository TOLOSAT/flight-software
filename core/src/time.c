/**
 * @file    time.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "time.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define SECONDS_IN_DAY 86400u         /**< Number of seconds in a day */
#define SECONDS_IN_HOUR 3600u         /**< Number of seconds in a hour */
#define SECONDS_IN_MINUTE 60u         /**< Number of seconds in a minute */
#define DAYS_IN_YEAR 365u             /**< Number of days in a year */
#define DAYS_IN_LEAP_YEAR 366u        /**< Number of days in a leap year (occures every 4 years execept some years) */
#define JANUARY_FIRST_2000 946684800u /**< UNIX timestamp for january 1rst 2000 (TOLOSAT RTC cannot compute time before this date) */

/**
 * @def  ARRAY_TO_UINT32_BIG_ENDIAN(array)
 * @brief Preprocessor function that convert 4 bytes array into uint32 big-endian variable
 */
#define ARRAY_TO_UINT32_BIG_ENDIAN(array) ((uint32_t)(((array)[0u] << 24) | ((array)[1u] << 16) | ((array)[2u] << 8) | ((array)[3u])))

/*************************** Functions Declarations **************************/

static coreStatus_t ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp);
static coreStatus_t ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time);
static coreStatus_t ConvertCUCTimeInChar(cucTime_t *cuc_time, char cuc_time_str[CUC_TIME_STR_SIZE]);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          GetStrCUCTime(char cuc_time_str[CUC_TIME_STR_SIZE])
 * @brief       Function that gets CUC time from RTC but as string (e.g. for printing)
 * @param[out]  cuc_time_str time formated according to CUC
 * @retval      #CORE_INVALID_PARAM if a pointer is NULL
 * @retval      #CORE_ERROR if an error occured
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t GetStrCUCTime(char cuc_time_str[CUC_TIME_STR_SIZE])
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    cucTime_t cuc_time = {0};

    // Function Core
    if (cuc_time_str != NULL)
    {
        // Get the CUC Time
        return_value = GetCUCTime(&cuc_time);
        if (return_value == CORE_SUCCESSFUL)
        {
            return_value = ConvertCUCTimeInChar(&cuc_time, cuc_time_str);
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          GetCUCTime(cucTime_t *cuc_time)
 * @brief       Function that gets CUC time from RTC
 * @param[out]  cuc_time time formated according to CUC
 * @retval      #CORE_INVALID_PARAM if a pointer is NULL
 * @retval      #CORE_ERROR if cannot get RTC time
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t GetCUCTime(cucTime_t *cuc_time)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
    rtcTime_t rtc_time = {0};

    // Function Core
    if (cuc_time != NULL)
    {
        // Get Time from RTC
        halStatus_t test_val = RtcGetTime(&rtc_time);
        if (test_val == GEN_HAL_SUCCESSFUL)
        {
            // Convert RTC to RAW CUC Time (TAI)
            uint32_t raw_cuc_time;
            return_value = ConvertRTCTimeToUnixTimestamp(rtc_time, &raw_cuc_time);
            if (return_value == CORE_SUCCESSFUL)
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
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetCUCTime(cucTime_t *cuc_time)
 * @brief       Function that sets RTC from CUC time
 * @param[out]  cuc_time time formated according to CUC
 * @retval      #CORE_INVALID_PARAM if a pointer is NULL
 * @retval      #CORE_ERROR if cannot set RTC time
 * @retval      #CORE_SUCCESSFUL else
 */
coreStatus_t SetCUCTime(cucTime_t *cuc_time)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
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
        if (return_value == CORE_SUCCESSFUL)
        {
            // Set Time from RTC
            halStatus_t test_val = RtcSetTime(&rtc_time);
            if (test_val != GEN_HAL_SUCCESSFUL)
            {
                return_value = CORE_ERROR;
            }
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          CompareCUCTimes(cucTime_t *older_cuc_time, cucTime_t *newer_cuc_time)
 * @brief       Compare two cuc_time.
 * @param[in]   older_cuc_time Presupposed older time
 * @param[in]   newer_cuc_time Presupposed newer time
 * @retval      #CORE_INVALID_PARAM if a pointer is NULL
 * @retval      #CORE_SUCCESSFUL if older_cuc_time =< newer_cuc_time
 * @retval      #CORE_ERROR if older_cuc_time > newer_cuc_time
 *
 * @warning This function assumes that COARSE_TIME_SIZE = 4 and FINE_TIME_SIZE = 1.
 */
coreStatus_t CompareCUCTimes(cucTime_t *older_cuc_time, cucTime_t *newer_cuc_time)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((older_cuc_time != NULL) && (newer_cuc_time != NULL))
    {
        uint32_t older_coarse_time = ARRAY_TO_UINT32_BIG_ENDIAN(older_cuc_time->coarse_time);
        uint32_t newer_coarse_time = ARRAY_TO_UINT32_BIG_ENDIAN(newer_cuc_time->coarse_time);
        if (older_coarse_time < newer_coarse_time)
        {
            return_value = CORE_SUCCESSFUL;
        }
        else if (older_coarse_time > newer_coarse_time)
        {
            return_value = CORE_ERROR;
        }
        else
        {
            // Coarse time are equal we need to check fine time
            if (older_cuc_time->fine_time[0] <= newer_cuc_time->fine_time[0])
            {
                return_value = CORE_SUCCESSFUL;
            }
            else
            {
                return_value = CORE_ERROR;
            }
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
 * @brief       Function that convert RTC time into Unix timestamp
 * @param[in]   rtc_time RTC time (as it has been defined in GENERIC HAL)
 * @param[out]  unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @retval      #CORE_INVALID_PARAM if a timestamp is NULL
 * @retval      #CORE_SUCCESSFUL else
 */
static coreStatus_t ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

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
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
 * @brief       Function that convert Unix timestamp into RTC time
 * @param[in]   unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @param[out]  rtc_time RTC time (as it has been defined in GENERIC HAL)
 * @retval      #CORE_INVALID_PARAM if a rtc_time is NULL or timestamp is before january 1rst 2000
 * @retval      #CORE_ERROR if RTC time has not been computed correctly
 * @retval      #CORE_SUCCESSFUL else
 */
static coreStatus_t ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;
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
            return_value = CORE_ERROR;
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertCUCTimeInChar(cucTime_t *cuc_time, char cuc_time_str[CUC_TIME_STR_SIZE])
 * @brief       This function converts cuc_time into a string
 * @param[in]   cuc_time        CUC time
 * @param[out]  cuc_time_str    CUC time but string formatted
 * @retval      #CORE_INVALID_PARAM if a pointer is NULL
 * @retval      #CORE_SUCCESSFUL else
 */
static coreStatus_t ConvertCUCTimeInChar(cucTime_t *cuc_time, char cuc_time_str[CUC_TIME_STR_SIZE])
{
    // Variable Initialisation
    coreStatus_t return_value = CORE_SUCCESSFUL;

    // Function Core
    if ((cuc_time_str != NULL) && (cuc_time))
    {
        uint8_t *cuc_time_ptr = (uint8_t *)cuc_time;
        for (uint32_t i = 0u; i < CUC_TIME_SIZE; i++)
        {
            // Convert first 4 bits
            uint8_t byte_msb = (cuc_time_ptr[i] & 0xF0u) >> 4u;
            if (byte_msb <= 0x09u)
            {
                cuc_time_str[2*i] = (byte_msb) + '0';
            }
            else if ((byte_msb >= 0x0Au) && (byte_msb <= 0x0Fu))
            {
                cuc_time_str[2*i] = (byte_msb - 0x0Au) + 'A';
            }
            else
            {
                /* Do Nothing */
            }
            

            // Convert last 4 bits
            uint8_t byte_lsb = cuc_time_ptr[i] & 0x0Fu;
            if (byte_lsb <= 0x09u)
            {
                cuc_time_str[2*i+1] = (byte_lsb) + '0';
            }
            else if ((byte_lsb >= 0x0Au) && (byte_lsb <= 0x0Fu))
            {
                cuc_time_str[2*i+1] = (byte_lsb - 0x0Au) + 'A';
            }
            else
            {
                /* Do Nothing */
            }
        }
    }
    else
    {
        return_value = CORE_INVALID_PARAM;
    }

    return return_value;
}