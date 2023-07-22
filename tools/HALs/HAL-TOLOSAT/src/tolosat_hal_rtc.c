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

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              RtcInit(rtcInst_t *rtc_inst)
 * @brief           Function that initialise RTC
 * @param[in,out]   rtc_inst Instance that contains RTC parameters
 * @retval          #FCT_SUCCESSFUL if creation succeed
 * @retval          #FCT_INVALID_PARAM if rtc_inst is a null pointer
 */
halStatus_t RtcInit(rtcInst_t *rtc_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    if (rtc_inst != NULL)
    {
        // Initialize RTC Only
        rtc_inst->handle_struct.Instance = RTC;
        rtc_inst->handle_struct.Init.HourFormat = RTC_HOURFORMAT_24;
        rtc_inst->handle_struct.Init.AsynchPrediv = 127u;
        rtc_inst->handle_struct.Init.SynchPrediv = 255u;
        rtc_inst->handle_struct.Init.OutPut = RTC_OUTPUT_DISABLE;
        rtc_inst->handle_struct.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
        rtc_inst->handle_struct.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
        rtc_inst->handle_struct.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
        test_val = HAL_RTC_Init(&rtc_inst->handle_struct);
        if (test_val == HAL_OK)
        {
            // Set Time
            sTime.Hours = 0u;
            sTime.Minutes = 0u;
            sTime.Seconds = 0u;
            sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
            sTime.StoreOperation = RTC_STOREOPERATION_RESET;
            test_val = HAL_RTC_SetTime(&rtc_inst->handle_struct, &sTime, RTC_FORMAT_BIN);
            if (test_val == HAL_OK)
            {
                // Set Date
                sDate.Date = 1u;
                sDate.Month = 1u;
                sDate.Year = 0u;
                test_val = HAL_RTC_SetDate(&rtc_inst->handle_struct, &sDate, RTC_FORMAT_BIN);
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
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RtcSetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time)
 * @brief       Function that sets time from RTC
 * @param[in]   rtc_inst
 * @param[in]   rtc_time
 * @retval      #FCT_INVALID_PARAM if a pointer is NULL
 * @retval      #FCT_ERROR if could not set RTC
 * @retval      #FCT_SUCCESSFUL else
 */
halStatus_t RtcSetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date;
    HAL_StatusTypeDef test_val;

    // Function Core
    if ((rtc_inst != NULL) && (rtc_time != NULL))
    {
        // Update time and date values
        date.Year = rtc_time->Year;
        date.Month = rtc_time->Month;
        date.Date = rtc_time->Day;
        time.Hours = rtc_time->Hours;
        time.Minutes = rtc_time->Minutes;
        time.Seconds = rtc_time->Seconds;
        test_val = HAL_RTC_SetDate(&rtc_inst->handle_struct, &date, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_SetTime(&rtc_inst->handle_struct, &time, RTC_FORMAT_BIN);
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
 * @fn          RtcGetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time)
 * @brief       Function that gets time from RTC
 * @param[in]   rtc_inst
 * @param[out]  rtc_time
 * @retval      #FCT_INVALID_PARAM if a pointer is NULL
 * @retval      #FCT_ERROR if could not read RTC
 * @retval      #FCT_SUCCESSFUL else
 */
halStatus_t RtcGetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    RTC_TimeTypeDef time = {0};
    RTC_DateTypeDef date = {0};
    HAL_StatusTypeDef test_val;

    // Function Core
    if ((rtc_inst != NULL) && (rtc_time != NULL))
    {
        test_val = HAL_RTC_GetDate(&rtc_inst->handle_struct, &date, RTC_FORMAT_BIN);
        if (test_val == HAL_OK)
        {
            test_val = HAL_RTC_GetTime(&rtc_inst->handle_struct, &time, RTC_FORMAT_BIN);
            if (test_val == HAL_OK)
            {
                // Update rtc_time values
                rtc_time->Year = date.Year;
                rtc_time->Month = date.Month;
                rtc_time->Day = date.Date;
                rtc_time->Hours = time.Hours;
                rtc_time->Minutes = time.Minutes;
                rtc_time->Seconds = time.Seconds;
                rtc_time->SubSeconds = 0;
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