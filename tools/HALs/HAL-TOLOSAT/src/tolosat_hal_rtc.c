/**
 * @file    tolosat_hal_rtc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL RTC functions
 * @date    18/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

#define RTC_DEFAULT_YEAR        0u      /**< Default year alias 2000 */
#define RTC_DEFAULT_MONTH       1u      /**< Default month alias january */
#define RTC_DEFAULT_DAY         1u      /**< Default day alias 1rst */
#define RTC_DEFAULT_HOUR        0u      /**< Default hour alias 0 */
#define RTC_DEFAULT_MINUTE      0u      /**< Default minute alias 0 */
#define RTC_DEFAULT_SECOND      0u      /**< Default second alias 0 */
#define MILLISECOND_SCALER      1000u   /**< Scaler to obtain millisecond precision time */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static rtcHandleStruct_t rtc_inst = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn      RtcInit(void)
 * @brief   Function that initialise RTC
 * @retval  #THAL_ERROR if cannot init RTC
 * @retval  #THAL_SUCCESSFUL else
 */
halStatus_t RtcInit(void)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    // Initialize RTC Only
    rtc_inst.Instance = RTC;
    TAPAS_RTC_SPECIFIC_INIT(rtc_inst);
    test_val = HAL_RTC_Init(&rtc_inst);
    if (test_val == HAL_OK)
    {
        // Set Time
        sTime.Hours = RTC_DEFAULT_HOUR;
        sTime.Minutes = RTC_DEFAULT_MINUTE;
        sTime.Seconds = RTC_DEFAULT_SECOND;
        test_val = HAL_RTC_SetTime(&rtc_inst, &sTime, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            // Set Date
            sDate.Date = RTC_DEFAULT_DAY;
            sDate.Month = RTC_DEFAULT_MONTH;
            sDate.Year = RTC_DEFAULT_YEAR;
            test_val = HAL_RTC_SetDate(&rtc_inst, &sDate, RTC_FORMAT_BIN);
            if (test_val != HAL_OK)
            {
                return_value = THAL_ERROR;
            }
        }
        else
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_ERROR;
    }

    return return_value;
}

/**
 * @fn          RtcSetTime(rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time Value of RTC time we want to set
 * @retval      #THAL_INVALID_PARAM if a pointer is NULL
 * @retval      #THAL_ERROR if could not set RTC
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t RtcSetTime(rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    if (rtc_time != NULL)
    {
        // Update time and date values
        date.Year = rtc_time->year;
        date.Month = rtc_time->month;
        date.Date = rtc_time->day;
        time.Hours = rtc_time->hour;
        time.Minutes = rtc_time->minute;
        time.Seconds = rtc_time->second;
        test_val = HAL_RTC_SetTime(&rtc_inst, &time, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_SetDate(&rtc_inst, &date, RTC_FORMAT_BIN);
            if (test_val != HAL_OK)
            {
                return_value = THAL_ERROR;
            }
        }
        else
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time Value to RTC time we want to read
 * @retval      #THAL_INVALID_PARAM if a pointer is NULL
 * @retval      #THAL_ERROR if could not read RTC
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t RtcGetTime(rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    if (rtc_time != NULL)
    {
        test_val = HAL_RTC_GetTime(&rtc_inst, &time, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_GetDate(&rtc_inst, &date, RTC_FORMAT_BIN);
            if (test_val == HAL_OK)
            {
                // Update rtc_time values
                rtc_time->year = date.Year;
                rtc_time->month = date.Month;
                rtc_time->day = date.Date;
                rtc_time->hour = time.Hours;
                rtc_time->minute = time.Minutes;
                rtc_time->second = time.Seconds;
                TAPAS_RTC_SET_MILLISEC(rtc_time);
            }
            else
            {
                return_value = THAL_ERROR;
            }
        }
        else
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}