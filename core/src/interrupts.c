/**
 * @file    interrupts.c
 * @author  Merlin Kooshmanian
 * @brief   Source file containing all the interrupts
 * @date    16/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "interrupts.h"
#include "generic_hal.h"
#include "io_instances.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief This function is the BUTTON interruption handler.
 */
void USER_BUTTON_IRQ_HANDLER(void)
{
    // First clear interrupt flag
    if (__HAL_GPIO_EXTI_GET_IT(USER_BUTTON_PIN) != 0x00U)
    {
    __HAL_GPIO_EXTI_CLEAR_IT(USER_BUTTON_PIN);
    }

    // Then do the interrupt routine
    /* Do something here */
}

/**
 * @brief This function handles I2C_AVIONIC event interrupt.
 */
void TAPAS_I2C_AVIONIC_EVT_IRQ_HANDLER(void)
{
    HAL_I2C_EV_IRQHandler(&iic_avionic_inst.handle_struct);
}

/**
 * @brief This function handles USART_PL global interrupt.
 */
void TAPAS_UART_PL_IRQ_HANDLER(void)
{
    HAL_UART_IRQHandler(&uart_pl_inst.handle_struct);
}

/**
 * @brief This function handles USART_TMTC global interrupt.
 */
void TAPAS_UART_TMTC_IRQ_HANDLER(void)
{
    HAL_UART_IRQHandler(&uart_tmtc_inst.handle_struct);
}

/**
 * @brief This function handles RX DMA for USART_TMTC global interrupt.
 */
void TAPAS_UART_TMTC_DMA_RX_IRQ_HANDLER(void)
{
    HAL_DMA_IRQHandler(&TAPAS_UART_TMTC_DMA_RX);
}

/**
 * @brief This function handles TX DMA for USART_TMTC global interrupt.
 */
void TAPAS_UART_TMTC_DMA_TX_IRQ_HANDLER(void)
{
    HAL_DMA_IRQHandler(&TAPAS_UART_TMTC_DMA_TX);
}