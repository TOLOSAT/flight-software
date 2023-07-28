/**
 * @file tolosat_hal_rtc.c
 * @author Merlin Kooshmanian
 * @brief Source file for TOLOSAT HAL RTC functions
 * @date 18/07/2023
 *
 * Last Update : 18/07/2023
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
 * @retval  #FCT_ERROR if cannot init RTC
 * @retval  #FCT_SUCCESSFUL else
 */
halStatus_t RtcInit(void)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    // Initialize RTC Only
#if defined(STM32H745xx)
    rtc_inst.Instance = RTC;
    rtc_inst.Init.HourFormat = RTC_HOURFORMAT_24;
    rtc_inst.Init.AsynchPrediv = 127u;
    rtc_inst.Init.SynchPrediv = 255u;
    rtc_inst.Init.OutPut = RTC_OUTPUT_DISABLE;
    rtc_inst.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    rtc_inst.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
    rtc_inst.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
#elif defined(STM32F411xE)
    rtc_inst.Instance = RTC;
    rtc_inst.Init.HourFormat = RTC_HOURFORMAT_24;
    rtc_inst.Init.AsynchPrediv = 127u;
    rtc_inst.Init.SynchPrediv = 255u;
    rtc_inst.Init.OutPut = RTC_OUTPUT_DISABLE;
    rtc_inst.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    rtc_inst.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
#elif defined(STM32F103xB)
    rtc_inst.Instance = RTC;
    rtc_inst.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    rtc_inst.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
#else
#error "Board is not supported"
#endif
    test_val = HAL_RTC_Init(&rtc_inst);
    if (test_val == HAL_OK)
    {
        // Set Time
        sTime.Hours = RTC_DEFAULT_HOUR;
        sTime.Minutes = RTC_DEFAULT_MINUTE;
        sTime.Seconds = RTC_DEFAULT_SECOND;
#if defined(STM32H745xx) || defined(STM32F411xE)
        sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sTime.StoreOperation = RTC_STOREOPERATION_RESET;
#elif defined(STM32F103xB)
#else
#error "Board is not supported"
#endif
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
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_ERROR;
        }
    }
    else
    {
        return_value = FCT_ERROR;
    }

    return return_value;
}

/**
 * @fn          RtcSetTime(rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time Value of RTC time we want to set
 * @retval      #FCT_INVALID_PARAM if a pointer is NULL
 * @retval      #FCT_ERROR if could not set RTC
 * @retval      #FCT_SUCCESSFUL else
 */
halStatus_t RtcSetTime(rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
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
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_ERROR;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time Value to RTC time we want to read
 * @retval      #FCT_INVALID_PARAM if a pointer is NULL
 * @retval      #FCT_ERROR if could not read RTC
 * @retval      #FCT_SUCCESSFUL else
 */
halStatus_t RtcGetTime(rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
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
#if defined(STM32H745xx) || defined(STM32F411xE)
                rtc_time->millisecond = (MILLISECOND_SCALER*(time.SecondFraction-time.SubSeconds))/(time.SecondFraction+1);
#elif defined(STM32F103xB)
                rtc_time->millisecond = 0u;
#else
#error "Board is not supported"
#endif
            }
            else
            {
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_ERROR;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}