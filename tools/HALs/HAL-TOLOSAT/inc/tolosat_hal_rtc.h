/**
 * @file tolosat_hal_rtc.h
 * @author Merlin Kooshmanian
 * @brief Header file for TOLOSAT HAL RTC functions
 * @date 18/07/2023
 *
 * Last Update : 18/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_rtc Driver RTC
 * Functions that allows to drive Real Time Clock
 * @{
 */

#ifndef TOLOSAT_HAL_RTC_H
#define TOLOSAT_HAL_RTC_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief RTC handle struct type redefinition */
typedef RTC_HandleTypeDef rtcHandleStruct_t;

/** 
 * @struct  rtcTime_t
 * @brief   Struct type definition for Time storage struct
 */
typedef struct
{
    uint8_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Seconds;
    uint32_t SubSeconds;
} rtcTime_t;

/** 
 * @struct  rtcInst_t
 * @brief   Struct type definition of a RTC instance
 */
typedef struct
{
    rtcHandleStruct_t handle_struct;    /**< @brief RTC handle struct used by ST HAL */
} rtcInst_t;

/**************************** Functions Prototypes ***************************/

halStatus_t RtcInit(rtcInst_t *rtc_inst);
halStatus_t RtcSetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time);
halStatus_t RtcGetTime(rtcInst_t *rtc_inst, rtcTime_t *rtc_time);


#endif /* TOLOSAT_HAL_RTC_H */

/** 
 * @} 
 * @} 
 */