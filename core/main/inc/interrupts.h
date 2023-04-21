/**
 * @file interrupts.h
 * @author Merlin Kooshmanian
 * @brief Header file containing all the interrupts
 * @date 16/04/2023
 * 
 * Last Update : 21/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

/***************************** Include Files *********************************/

#include "stm32f4xx_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void TIM1_TRG_COM_TIM11_IRQHandler(void);
void SysTick_Handler(void);

#endif /* __STM32F4xx_IT_H */
