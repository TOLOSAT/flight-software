/**
 * @file    drv_rtc.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for RTC functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_rtc.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define RTC_DEFAULT_YEAR   0u    /**< Default year alias 2000 */
#define RTC_DEFAULT_MONTH  2u    /**< Default month alias february */
#define RTC_DEFAULT_DAY    17u   /**< Default day alias 17th */
#define RTC_DEFAULT_HOUR   11u   /**< Default hour alias 13h */
#define RTC_DEFAULT_MINUTE 30u   /**< Default minute alias 30m */
#define RTC_DEFAULT_SECOND 0u    /**< Default second alias 0 */
#define MILLISECOND_SCALER 1000u /**< Scaler to obtain millisecond precision time */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

static RTC_HandleTypeDef rtc_inst = { 0 };

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitRtc(void)
 * @brief   Function that initialise RTC
 * @return  Nothing
 */
void InitRtc(void)
{
    RTC_TimeTypeDef sTime = { 0 };
    RTC_DateTypeDef sDate = { 0 };
    HAL_StatusTypeDef test_val;

    // Initialize RTC parameters
    rtc_inst.Instance            = RTC;
    rtc_inst.Init.HourFormat     = RTC_HOURFORMAT_24;
    rtc_inst.Init.OutPut         = RTC_OUTPUT_DISABLE;
    rtc_inst.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    rtc_inst.Init.OutPutType     = RTC_OUTPUT_TYPE_OPENDRAIN;
#if defined(STM32H7)
    rtc_inst.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
#endif
    // Prescaler need to be chosen according to the following formulae :
    // 1 Hz = RTC_CLOCK / ((PREDIV_A + 1) * (PREDIV_S + 1))
    // The higher the PREDIV_A the lower the consumption
    // The higher the PREDIV_S the higher the precision
    // In our case precision is more important than few uW
    rtc_inst.Init.AsynchPrediv = 1u;     // PREDIV_A
    rtc_inst.Init.SynchPrediv  = 16383u; // PREDIV_S

    // Start RTC
    test_val = HAL_RTC_Init(&rtc_inst);
    if (test_val == HAL_OK)
    {
        // Set Time
        sTime.Hours   = RTC_DEFAULT_HOUR;
        sTime.Minutes = RTC_DEFAULT_MINUTE;
        sTime.Seconds = RTC_DEFAULT_SECOND;
        test_val      = HAL_RTC_SetTime(&rtc_inst, &sTime, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            // Set Date
            sDate.Date  = RTC_DEFAULT_DAY;
            sDate.Month = RTC_DEFAULT_MONTH;
            sDate.Year  = RTC_DEFAULT_YEAR;
            test_val    = HAL_RTC_SetDate(&rtc_inst, &sDate, RTC_FORMAT_BIN);
            if (test_val != HAL_OK)
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }
}

/**
 * @fn          RtcSetTime(const rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_time    Value of RTC time we want to set
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t RtcSetTime(const rtcTime_t *rtc_time)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    RTC_TimeTypeDef time      = { 0 };
    RTC_DateTypeDef date      = { 0 };
    HAL_StatusTypeDef test_val;

    // Check parameter(s)
    if (rtc_time != NULL)
    {
        // Update time and date values
        date.Year    = rtc_time->year;
        date.Month   = rtc_time->month;
        date.Date    = rtc_time->day;
        time.Hours   = rtc_time->hour;
        time.Minutes = rtc_time->minute;
        time.Seconds = rtc_time->second;
        test_val     = HAL_RTC_SetTime(&rtc_inst, &time, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_SetDate(&rtc_inst, &date, RTC_FORMAT_BIN);
            if (test_val != HAL_OK)
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RtcGetTime(rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[out]  rtc_time    Value to RTC time we want to read
 * @retval      #RET_INVALID_PARAM if a pointer is NULL
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t RtcGetTime(rtcTime_t *rtc_time)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    RTC_TimeTypeDef time      = { 0 };
    RTC_DateTypeDef date      = { 0 };
    HAL_StatusTypeDef test_val;

    // Check parameter(s)
    if (rtc_time != NULL)
    {
        test_val = HAL_RTC_GetTime(&rtc_inst, &time, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_GetDate(&rtc_inst, &date, RTC_FORMAT_BIN);
            if (test_val == HAL_OK)
            {
                // Update rtc_time values
                rtc_time->year      = date.Year;
                rtc_time->month     = date.Month;
                rtc_time->day       = date.Date;
                rtc_time->hour      = time.Hours;
                rtc_time->minute    = time.Minutes;
                rtc_time->second    = time.Seconds;
                rtc_time->subsecond = ((time.SecondFraction - time.SubSeconds) << 16) / (time.SecondFraction + 1u);
            }
            else
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}