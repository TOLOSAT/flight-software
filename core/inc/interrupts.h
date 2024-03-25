/**
 * @file    interrupts.h
 * @author  Merlin Kooshmanian
 * @brief   Header file containing all the interrupts
 * @date    16/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);

#endif /* INTERRUPTS_H */
