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

#if defined(STM32F411xE)
#include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#elif defined(STM32H745xx)
#include "stm32h7xx_hal.h"
#else
#error "Board is not supported"
#endif

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM4_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void I2C1_EV_IRQHandler(void);
void USART6_IRQHandler(void);
void USART1_IRQHandler(void);
#if defined(STM32F411xE)
void DMA2_Stream2_IRQHandler(void);
void DMA2_Stream7_IRQHandler(void);
#elif defined(STM32F103xB)
void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel5_IRQHandler(void);
#elif defined(STM32H745xx)
void DMA1_Stream0_IRQHandler(void);
void DMA1_Stream1_IRQHandler(void);
#else 
#error "Board is not supported"
#endif

#endif /* __STM32F4xx_IT_H */
