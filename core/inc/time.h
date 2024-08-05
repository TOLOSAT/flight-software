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
#include <assert.h>

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

#define CUC_HEAD_SIZE                   1u                                                  /**< CUC header size */
#define COARSE_TIME_SIZE                4u                                                  /**< Coarse time size */
#define FINE_TIME_SIZE                  1u                                                  /**< Fine time size */
#define CUC_TIME_SIZE                   (CUC_HEAD_SIZE + COARSE_TIME_SIZE + FINE_TIME_SIZE) /**< CUC time variables size */
#define TIME_HEAD_CONSTANT              0x1du                                               /**< P-field for CUC time (equivalent of 0b00011111u)*/
#define TAI_UNIX_OFFSET                 378691200u                                          /**< Number of seconds between TAI Ref (January 1rst 1958) and UNIX Ref (January 1rst 1970) */
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

/** 
 * @enum    timeStatus_t
 * @brief   Time functions specific returns 
 */
typedef enum
{
    TIME_SUCCESSFUL            = 0u,    /**< Function succeed */
    TIME_ERROR                 = 1u,    /**< Function failed */
    TIME_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    TIME_TIMEOUT               = 3u,    /**< Function returned a timeout */
} timeStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern timeStatus_t GetStrCUCTime(char cuc_time_str[CUC_TIME_STR_SIZE]);
extern timeStatus_t GetCUCTime(cucTime_t *cuc_time);
extern timeStatus_t SetCUCTime(cucTime_t *cuc_time);
extern timeStatus_t CompareCUCTimes(cucTime_t *older_cuc_time, cucTime_t *newer_cuc_time);

#endif /* TIME_H */
