/**
 * @file    generic_hal_console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL Console functions
 * @date    23/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup generic_hal GENERIC HAL
 * @{
 * @defgroup console Console
 * Functions that allows to print into a console 
 * (either using UART, a file or a memory)
 * @{
 */

#ifndef GENERIC_HAL_CONSOLE_H
#define GENERIC_HAL_CONSOLE_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void ConsoleOpen(void);
void ConsolePrint(const char *format, ...);

#endif /* GENERIC_HAL_CONSOLE_H */

/** 
 * @} 
 * @} 
 */