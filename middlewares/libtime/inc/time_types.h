/**
 * @file    time_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header for time types
 * @date    02/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TIME_TYPES_H
#define TIME_TYPES_H

/******************************* Include Files *******************************/

#include <assert.h>
#include <stdint.h>

#include "time_constants.h"

/***************************** Macros Definitions ****************************/

#if !defined(BYTE_ALIGNED) && !defined(ASSERT_SIZE)
#define BYTE_ALIGNED                __attribute__((packed, aligned(1)))                                                 /**< Preprocessor function that force byte alignment for struct */
#define ASSERT_SIZE(object, size)   static_assert((sizeof(object) == (size)), "Object has not the expected size !");    /**< Preprocessor function that ensure objects have the expected size */
#endif

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

#endif /* TIME_TYPES_H */
