/**
 * @file    interrupts.h
 * @author  Merlin Kooshmanian
 * @brief   Header file containing all the interrupts
 * @date    16/04/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

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
void TIM4_IRQHandler(void);
void USER_BUTTON_IRQ_HANDLER(void);
void USER_BUTTON_IRQ_CALLBACK(uint16_t GPIO_Pin);
void TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER(void);
void TAPAS_UART_PL_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER(void);
void TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER(void);

#endif /* INTERRUPTS_H */
