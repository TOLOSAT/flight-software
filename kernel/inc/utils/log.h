/**
 * @file    log.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for log
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup log Log
 * @brief Functions for handling log message.
 * @{
 */

#ifndef LOG_H
#define LOG_H

#include "kernel_types.h"

/**
 * @def     LOG(msg)
 * @brief   Print a message in the console.
 */
#define LOG(msg)                          ConsolePrint(msg, 0, 0u, 0.0f, 0u)

/**
 * @def     LOG_DECIMAL(msg,number)
 * @brief   Prints a message in the console with a signed decimal integer.
 */
#define LOG_DECIMAL(msg, number)          ConsolePrint(msg, number, 0u, 0.0f, 0u)

/**
 * @def     LOG_HEXDECIMAL(msg,number)
 * @brief   Prints a message in the console with a unsigned hexadecimal integer.
 */
#define LOG_HEXDECIMAL(msg, number)       ConsolePrint(msg, 0, number, 0.0f, 0u)

/**
 * @def     LOG_FLOAT(msg,number,precision)
 * @brief   Prints a message in the console with a floating point number.
 */
#define LOG_FLOAT(msg, number, precision) ConsolePrint(msg, 0, 0u, number, precision)

#endif /* LOG_H */

/**
 * @}
 * @}
 * @}
 */