/**
 * @file    time.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for Time Management
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/time.h"
#include "core/tasks.h"
#include "drv/others/drv_rtc.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/**
 * @def  ARRAY_TO_UINT32_BIG_ENDIAN(array)
 * @brief Preprocessor function that convert 4 bytes array into uint32 big-endian variable
 */
#define ARRAY_TO_UINT32_BIG_ENDIAN(array) ((uint32_t)(((array)[0u] << 24) | ((array)[1u] << 16) | ((array)[2u] << 8) | ((array)[3u])))

/*************************** Functions Declarations **************************/

static uint32_t ConvertRTCTimeToTAITimestamp(rtcTime_t rtc_time);
static rtcTime_t ConvertTAITimestampToRTCTime(uint32_t tai_timestamp);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          GetTick(void)
 * @brief       Function that returns how many tick occured since the scheduler started
 * @return      Ticks
 */
tick_t GetTick(void)
{
    return (tick_t)xTaskGetTickCount();
}

/**
 * @fn          Sleep(tick_t tick)
 * @brief       Function that puts to sleep the current task.
 * @param[in]   tick    Amount of time the task will be put to sleep.
 * @return      Nothing
 *
 * @note Using tick = 0 will make the task yielding instead.
 */
void Sleep(tick_t tick)
{
    // Gets current task no
    taskNo_t current_task = GetCurrentTask();
    if (current_task != NO_TASK)
    {
        // Check First if a suspension is require or not
        if (g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].mode == TASK_SUSPENDED)
        {
            // Suspend the task
            vTaskSuspend(g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].handle);
        }
        else
        {
            // Yielding instead of sleeping when tick equal to zero
            if (tick == 0u)
            {
                taskYIELD();
            }
            else
            {
                vTaskDelay(tick);
            }

            // Check if task has not been suspended during the sleep
            if (g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].mode == TASK_SUSPENDED)
            {
                // Suspend the task
                vTaskSuspend(g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].handle);
            }
        }
    }
}

/**
 * @fn      SleepPeriodic(void)
 * @brief   Function that puts to sleep the current task until next period
 * @return  Nothing
 */
void SleepPeriodic(void)
{
    // Gets current task no
    taskNo_t current_task = GetCurrentTask();
    if (current_task != NO_TASK)
    {
        // Check First if a suspension is require or not
        if (g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].mode == TASK_SUSPENDED)
        {
            // Suspend the task
            vTaskSuspend(g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].handle);
        }
        else
        {
            // Before sleeping check if we missed period
            tick_t current_tick = xTaskGetTickCount();
            tick_t next_period  = g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].last_wake // Last time the task wakeup
                                 + g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].period;  // + the task period
            if (current_tick <= next_period)
            {
                // If period not missed, wait until next period
                xTaskDelayUntil(&g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].last_wake,
                                g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].period);
            }
            else
            {
                // Yield instead
                taskYIELD();
            }

            // Check if task has not been suspended during the sleep
            if (g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].mode == TASK_SUSPENDED)
            {
                // Suspend the task
                vTaskSuspend(g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].handle);
            }
        }

        // Update last wake time anyway
        g_tasks_desc_table[TASKNO_TO_LINENO(current_task)].last_wake = xTaskGetTickCount();
    }
}

/**
 * @fn          GetTime(void)
 * @brief       Function that gets time (in CUC format) from RTC
 * @return      CUC formated time
 */
time_t GetTime(void)
{
    time_t time        = INVALID_TIME;
    rtcTime_t rtc_time = { 0 };

    // Get Time from RTC
    returnCode_t test_val = RtcGetTime(&rtc_time);
    if (test_val == RET_SUCCESSFUL)
    {
        // Convert RTC to RAW CUC Time (TAI)
        uint32_t tai_timestamp = ConvertRTCTimeToTAITimestamp(rtc_time);

        // Convert RAW CUC Time to CUC Time
        // Byte 7     : CUC P-Field (constant)
        // Byte 6 - 3 : CUC Basic Time (time in second elapsed since epoch time (1rst of January 1958))
        // Byte 2 - 0 : CUC Fractionnal Time (2^(-n) second elapsed)
        // Note : Here byte 0 & 1 always equal zero because we are not precise enough
        time_t p_field         = ((uint64_t)P_FIELD_CONSTANT & 0xffu) << P_FIELD_OFFSET;
        time_t basic_time      = ((uint64_t)tai_timestamp) << BASIC_TIME_OFFSET;
        time_t fractional_time = ((uint64_t)rtc_time.subsecond) << FRACTIONAL_TIME_OFFSET;

        time = (time_t)(p_field | basic_time | fractional_time);
    }
    else
    {
        KernelPanic();
    }

    return time;
}

/**
 * @fn          SetTime(time_t time)
 * @brief       Function that sets RTC from a time value (in CUC format)
 * @param[out]  time    Time formated according to CUC
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t SetTime(time_t time)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Get cuc time header
    uint8_t cuc_time_header = (uint8_t)((time & P_FIELD_MASK) >> P_FIELD_OFFSET);
    if (cuc_time_header == P_FIELD_CONSTANT)
    {
        // Get TAI timestamp from CUC Time
        uint32_t tai_timestamp = (uint32_t)((time & BASIC_TIME_MASK) >> BASIC_TIME_OFFSET);
        if (tai_timestamp >= JANUARY_FIRST_2000)
        {
            // Convert TAI timestamp to RTC Time
            rtcTime_t rtc_time = ConvertTAITimestampToRTCTime(tai_timestamp);

            // Set Time from RTC
            return_value = RtcSetTime(&rtc_time);
            if (return_value != RET_SUCCESSFUL)
            {
                KernelPanic();
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          ConvertRTCTimeToTAITimestamp(rtcTime_t rtc_time)
 * @brief       Function that convert RTC time into TAI timestamp
 * @param[in]   rtc_time    RTC time (as it has been defined in drv RTC)
 * @return      TAI timestamp (number of seconds since january 1, 1958)
 */
static uint32_t ConvertRTCTimeToTAITimestamp(rtcTime_t rtc_time)
{
    uint32_t tai_timestamp = 0u;

    // Numbers of day each month
    const uint8_t DAYS_IN_MONTH[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Compute total of days elapsed since january 1rst 1958
    uint32_t total_days = (rtc_time.year + 42u) * DAYS_IN_YEAR;
    for (uint16_t i = 1958u; i < ((uint16_t)rtc_time.year + 2000u); i++)
    {
        if ((((i % 4u) == 0u) && ((i % 100u) != 0u)) || ((i % 400u) == 0u))
        {
            total_days++; // Add 1 day for leap years
        }
    }

    // Add days for each full month of the current year
    for (uint8_t i = 0u; i < (rtc_time.month - 1u); i++)
    {
        total_days += DAYS_IN_MONTH[i];
        if ((i == 1u) && ((((rtc_time.year % 4u) == 0u) && ((rtc_time.year % 100u) != 0u)) || ((rtc_time.year % 400u) == 0u)))
        {
            total_days++; // Add 1 day for february month during leap year
        }
    }

    // Add days elapsed in the current month
    total_days += rtc_time.day - 1u;

    // Total seconds elapsed computation
    tai_timestamp = (total_days * SECONDS_IN_DAY)           // Days
                    + (rtc_time.hour * SECONDS_IN_HOUR)     // Hours
                    + (rtc_time.minute * SECONDS_IN_MINUTE) // Minutes
                    + rtc_time.second;                      // Seconds

    return tai_timestamp;
}

/**
 * @fn          ConvertTAITimestampToRTCTime(uint32_t tai_timestamp)
 * @brief       Function that convert TAI timestamp into RTC time
 * @param[in]   tai_timestamp  Timestamp TAI (number of seconds since january 1, 1958)
 * @return      RTC time (as it has been defined in drv RTC)
 */
static rtcTime_t ConvertTAITimestampToRTCTime(uint32_t tai_timestamp)
{
    rtcTime_t rtc_time = { 0 };
    uint32_t timestamp = tai_timestamp;

    uint32_t year = 1958u;

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

    const uint8_t days_in_month[12] = { 31u, 28u, 31u, 30u, 31u, 30u, 31u, 31u, 30u, 31u, 30u, 31u };

    // Set RTC Time year field
    rtc_time.year = (uint8_t)(year % 100u);

    // Month and Month Day Calculation
    rtc_time.month = 1u;
    while ((rtc_time.month <= 12u) && (timestamp >= (SECONDS_IN_DAY * days_in_month[rtc_time.month - 1u])))
    {
        uint8_t days_in_current_month = days_in_month[rtc_time.month - 1u];
        if ((rtc_time.month == 2u) && ((((year % 4u) == 0u) && ((year % 100u) != 0u)) || ((year % 400u) == 0u)))
        {
            days_in_current_month = 29u;
        }
        timestamp -= (SECONDS_IN_DAY * days_in_current_month);
        rtc_time.month++;
    }
    rtc_time.day  = (timestamp / SECONDS_IN_DAY) + 1u;
    timestamp    %= SECONDS_IN_DAY;

    // Hour, Minute and Second Calculation
    rtc_time.hour    = timestamp / SECONDS_IN_HOUR;
    timestamp       %= SECONDS_IN_HOUR;
    rtc_time.minute  = timestamp / SECONDS_IN_MINUTE;
    rtc_time.second  = timestamp % SECONDS_IN_MINUTE;

    return rtc_time;
}
