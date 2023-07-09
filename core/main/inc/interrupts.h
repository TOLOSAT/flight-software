/**
 * @file interrupts.h
 * @author Merlin Kooshmanian
 * @brief Header file containing all the interrupts
 * @date 16/04/2023
 * 
 * Last Update : 27/05/2023
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

/*************************** Macros Definitions ******************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void TIM4_IRQHandler(void);
void USER_BUTTON_IRQ_HANDLER(void);
void TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER(void);
void TAPAS_UART_PL_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER(void);

#endif /* __STM32F4xx_IT_H */
