/**
 * @file    boot_interrupts.h
 * @author  Merlin Kooshmanian
 * @brief   Header file containing all the interrupts
 * @date    27/03/2024
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BOOT_INTERRUPTS_H
#define BOOT_INTERRUPTS_H

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

#endif /* BOOT_INTERRUPTS_H */
