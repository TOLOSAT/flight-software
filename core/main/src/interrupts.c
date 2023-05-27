/**
 * @file interrupts.c
 * @author Merlin Kooshmanian
 * @brief Source file containing all the interrupts
 * @date 16/04/2023
 *
 * Last Update : 27/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/
#include "interrupts.h"
#include "tolosat_hal.h"

#if defined(STM32F411xE)
#include "stm32f4xx_nucleo_bsp.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_nucleo_bsp.h"
#elif defined(STM32H745xx)
#include "stm32h7xx_nucleo_bsp.h"
#else
#error "Board is not supported"
#endif

/************************** Variable Definitions *****************************/

extern TIM_HandleTypeDef htim4;

extern iicInst_t iic_avionic_inst;
extern uartInst_t uart_pl_inst;
extern uartInst_t uart_tmtc_inst;

extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_RX;
extern DMA_HandleTypeDef TAPAS_UART_TMTC_DMA_TX;


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

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
 * @brief This function handles EXTernal Interrupt 0 handler.
 */
void USER_BUTTON_IRQ_HANDLER(void)
{
    HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
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