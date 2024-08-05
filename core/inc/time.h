/**
 * @file    time.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Time Management
 * @date    23/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TIME_H
#define TIME_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "core_types.h"

/***************************** Macros Definitions ****************************/

#define CUC_HEAD_SIZE                   1u                                                  /**< CUC header size */
#define COARSE_TIME_SIZE                4u                                                  /**< Coarse time size */
#define FINE_TIME_SIZE                  1u                                                  /**< Fine time size */
#define CUC_TIME_SIZE                   (CUC_HEAD_SIZE + COARSE_TIME_SIZE + FINE_TIME_SIZE) /**< CUC time variables size */
#define CUC_TIME_STR_SIZE               (2*CUC_TIME_SIZE)                                   /**< Number of char needed to represent CUC time as a string (2 char are need to represent 1 uint8_t)*/

/***************************** Types Definitions *****************************/

/**
 * @struct  cucTime_t
 * @brief   Struct type for CUC Time
 */
typedef struct
{
    uint8_t time_header;                    /**< @brief Field that helps to know which standard was choosen */
    uint8_t coarse_time[COARSE_TIME_SIZE];  /**< @brief Field that contains time in second since reference */
    uint8_t fine_time[FINE_TIME_SIZE];      /**< @brief Field that contains time fraction */
} BYTE_ALIGNED cucTime_t;
ASSERT_SIZE(cucTime_t, CUC_TIME_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t GetStrCUCTime(char cuc_time_str[CUC_TIME_STR_SIZE]);
extern coreStatus_t GetCUCTime(cucTime_t *cuc_time);
extern coreStatus_t SetCUCTime(cucTime_t *cuc_time);
extern coreStatus_t CompareCUCTimes(cucTime_t *older_cuc_time, cucTime_t *newer_cuc_time);

#endif /* TIME_H */
