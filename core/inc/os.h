/**
 * @file    os.h
 * @author  Merlin Kooshmanian
 * @brief   OS API header file
 * @date    27/10/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef OS_H
#define OS_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void StartOS(void);

#endif /* OS_H */