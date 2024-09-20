/**
 * @file    time.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/time.h"
#include "drv/drv_rtc.h"

/***************************** Macros Definitions ****************************/

#define TAI_UNIX_OFFSET         378691200u  /**< Number of seconds between TAI Ref (January 1rst 1958) and UNIX Ref (January 1rst 1970) */
#define TIME_HEADER_CONSTANT    0x1fu       /**< P-field for CUC time (equivalent of 0b00011111u)*/

#define P_FIELD_OFFSET          56                      /**< CUC P field offset */
#define P_FIELD_MASK            0xff00000000000000llu   /**< CUC P field mask */
#define BASIC_TIME_OFFSET       24                      /**< CUC basic time field offset */
#define BASIC_TIME_MASK         0x00ffffffff000000llu   /**< CUC basic time field mask */
#define FRACTIONAL_TIME_OFSSET  8                       /**< CUC fractional time field offset */
#define FRACTIONAL_TIME_MASK    0x0000000000ffffffllu   /**< CUC fractional time field mask */

#define SECONDS_IN_DAY          86400u      /**< Number of seconds in a day */
#define SECONDS_IN_HOUR         3600u       /**< Number of seconds in a hour */
#define SECONDS_IN_MINUTE       60u         /**< Number of seconds in a minute */
#define DAYS_IN_YEAR            365u        /**< Number of days in a year */
#define DAYS_IN_LEAP_YEAR       366u        /**< Number of days in a leap year (occures every 4 years execept some years) */
#define JANUARY_FIRST_2000      946684800u  /**< UNIX timestamp for january 1rst 2000 (TOLOSAT RTC cannot compute time before this date) */

/**
 * @def  ARRAY_TO_UINT32_BIG_ENDIAN(array)
 * @brief Preprocessor function that convert 4 bytes array into uint32 big-endian variable
 */
#define ARRAY_TO_UINT32_BIG_ENDIAN(array) ((uint32_t)(((array)[0u] << 24) | ((array)[1u] << 16) | ((array)[2u] << 8) | ((array)[3u])))

/*************************** Functions Declarations **************************/

static kernelStatus_t ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp);
static kernelStatus_t ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          GetTime(time_t *time)
 * @brief       Function that gets time (in CUC format) from RTC
 * @param[out]  time time formated according to CUC
 * @retval      #KERNEL_INVALID_PARAM if a pointer is NULL
 * @retval      #KERNEL_ERROR if cannot get RTC time
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION GetTime(time_t *time)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    rtcTime_t rtc_time = {0};

    // Function Core
    if (time != NULL)
    {
        // Get Time from RTC
        kernelStatus_t test_val = RtcGetTime(&rtc_time);
        if (test_val == KERNEL_SUCCESSFUL)
        {
            // Convert RTC to RAW CUC Time (TAI)
            uint32_t timestamp_sec;
            return_value = ConvertRTCTimeToUnixTimestamp(rtc_time, &timestamp_sec);
            if (return_value == KERNEL_SUCCESSFUL)
            {
                // Add TAI offset
                timestamp_sec += TAI_UNIX_OFFSET;

                // Convert RAW CUC Time to CUC Time
                // Byte 7     : CUC P-Field (constant)
                // Byte 6 - 3 : CUC Basic Time (time in second elapsed since epoch time (1rst of January 1958))
                // Byte 2 - 0 : CUC Fractionnal Time (2^(-n) second elapsed)
                // Note : Here byte 0 & 1 always equal zero because we are not precise enough
                time_t p_field = ((uint64_t)TIME_HEADER_CONSTANT & 0xffu) << P_FIELD_OFFSET;
                time_t basic_time = ((uint64_t)timestamp_sec) << BASIC_TIME_OFFSET;
                time_t fractional_time = ((uint64_t)rtc_time.subsecond) << FRACTIONAL_TIME_OFSSET;

                *time = (time_t)(p_field | basic_time | fractional_time);
            }
        }
        else
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SetTime(time_t time)
 * @brief       Function that sets RTC from a time value (in CUC format)
 * @param[out]  time time formated according to CUC
 * @retval      #KERNEL_INVALID_PARAM if a pointer is NULL
 * @retval      #KERNEL_ERROR if cannot set RTC time
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION SetTime(time_t time)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    rtcTime_t rtc_time = {0};

    // Function Core
    uint8_t cuc_time_header = (uint8_t)((time & P_FIELD_MASK) >> P_FIELD_OFFSET);
    if (cuc_time_header == TIME_HEADER_CONSTANT)
    {
        // Get UNIX Time from CUC Time
        uint32_t unix_time = (uint32_t)((time & BASIC_TIME_MASK) >> BASIC_TIME_OFFSET) - TAI_UNIX_OFFSET;
        // Convert UNIX Time to RTC Time
        return_value = ConvertUnixTimestampToRTCTime(unix_time, &rtc_time);
        if (return_value == KERNEL_SUCCESSFUL)
        {
            // Set Time from RTC
            kernelStatus_t test_val = RtcSetTime(&rtc_time);
            if (test_val != KERNEL_SUCCESSFUL)
            {
                return_value = KERNEL_ERROR;
            }
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
 * @brief       Function that convert RTC time into Unix timestamp
 * @param[in]   rtc_time RTC time (as it has been defined in drv RTC)
 * @param[out]  unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @retval      #KERNEL_INVALID_PARAM if a timestamp is NULL
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION ConvertRTCTimeToUnixTimestamp(rtcTime_t rtc_time, uint32_t *unix_timestamp)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

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
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
 * @brief       Function that convert Unix timestamp into RTC time
 * @param[in]   unix_timestamp Timestamp Unix (number of seconds since january 1, 1970)
 * @param[out]  rtc_time RTC time (as it has been defined in drv RTC)
 * @retval      #KERNEL_INVALID_PARAM if a rtc_time is NULL or timestamp is before january 1rst 2000
 * @retval      #KERNEL_ERROR if RTC time has not been computed correctly
 * @retval      #KERNEL_SUCCESSFUL else
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION ConvertUnixTimestampToRTCTime(uint32_t unix_timestamp, rtcTime_t *rtc_time)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
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
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}
