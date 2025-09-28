/**
 * @file    os.h
 * @author  Merlin Kooshmanian
 * @brief   OS API header file
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef OS_H
#define OS_H

/******************************* Include Files *******************************/

#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void StartOS(void);

#endif /* OS_H */