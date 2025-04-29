/**
 * @file    time_conversions.h
 * @author  Matteo Planchet
 * @brief   Header file for time conversions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup time_conversions Time Conversions
 * @brief Functions for time conversions.
 * @{
 */

#ifndef TIME_CONVERSIONS_H
#define TIME_CONVERSIONS_H

#include "kernel_types.h"

/**
 * @def     CUC_TO_TICK(cuc_time)
 * @brief   Convert CUC time format to tick in ms
 *
 * @note    1. Get the T-field from CUC by removing the P-field
 * @note    2. Convert to ms by multiplying by 1000
 * @note    3. Diviser par 2^24 pour normaliser
 * @note    4. Garder 32 bits en masquant par 0xffffffff
 */
#define CUC_TO_TICK(cuc_time) ((tick_t)(((((cuc_time) & (~P_FIELD_MASK)) * 1000u) / (1 << 24)) & 0xffffffffu))

#endif