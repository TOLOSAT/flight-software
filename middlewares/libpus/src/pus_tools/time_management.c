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

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

void ConvertRTCtoReadableTime(rtcTime_t *rtc_time, struct tm *readable_time);

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
    halStatus_t test_val;

    // Function Core
    if (cuc_time != NULL)
    {
        // Get Time from RTC
        test_val = RtcGetTime(&rtc_time);
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
            cuc_time->coarse_time[3] = (uint8_t)( 0x000000ffu & raw_cuc_time);
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
 * @fn          ConvertRTCtoReadableTime(rtcTime_t *rtc_time, struct tm *readable_time)
 * @brief       Function that convert rtc time into readable time (from libc)
 * @param[in]   rtc_time 
 * @param[out]  readable_time 
 */
void ConvertRTCtoReadableTime(rtcTime_t *rtc_time, struct tm *readable_time)
{
    readable_time->tm_year = (int)(rtc_time->year) + 2000 - 1900;
    readable_time->tm_mon = (int)(rtc_time->month) - 1;
    readable_time->tm_mday = (int)(rtc_time->day);
    readable_time->tm_hour = (int)(rtc_time->hour);
    readable_time->tm_min = (int)(rtc_time->minute);
    readable_time->tm_sec = (int)(rtc_time->second);
}
