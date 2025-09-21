/**
 * @file    signals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for signals handling functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup signals Signals
 * @brief Signals handling interface.
 * @{
 */

#ifndef SIGNALS_H
#define SIGNALS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

returnCode_t SendSignal(taskNo_t task, signalMask_t mask);
returnCode_t WaitSignal(signalMask_t mask);

#endif /* SIGNALS_H */

/**
 * @}
 * @}
 * @}
 */