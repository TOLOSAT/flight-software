/**
 * @file    console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup console Console
 * Functions that allows to print into a console 
 * (either using UART, a file or a memory)
 * @{
 */

#ifndef CONSOLE_H
#define CONSOLE_H

/******************************* Include Files *******************************/

#include "core_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void ConsolePrint(const char *msg);
void ConsolePrintNumber(signed int number);
void ConsolePrintHex(unsigned int hex);
void ConsolePrintFloat(float number, int precision);

#endif /* CONSOLE_H */

/** 
 * @} 
 * @} 
 */