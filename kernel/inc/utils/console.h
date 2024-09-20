/**
 * @file    console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup console Console
 * @brief Interface for printing and logging messages.
 * @{
 */

#ifndef CONSOLE_H
#define CONSOLE_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void ConsolePrint(const char *msg);
extern void ConsolePrintNumber(signed int number);
extern void ConsolePrintHex(unsigned int hex);
extern void ConsolePrintFloat(float number, int precision);

#endif /* CONSOLE_H */

/** 
 * @}
 * @}
 * @}
 */