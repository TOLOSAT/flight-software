/**
 * @file    console.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Console functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup console Console
 * @brief Interface for printing and LOGging messages.
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

extern void InitConsole(void);
extern void ConsolePrint(const char *msg, signed int dnumber, unsigned int hnumber, float fnumber, unsigned int fprecision);

#endif /* CONSOLE_H */

/** 
 * @}
 * @}
 * @}
 */