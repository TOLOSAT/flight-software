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

extern void NMI_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);
extern void DebugMon_Handler(void);

#endif /* BOOT_INTERRUPTS_H */
