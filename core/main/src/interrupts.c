/**
 * @file interrupts.c
 * @author Merlin Kooshmanian
 * @brief Source file containing all the interrupts
 * @date 16/04/2023
 *
 * Last Update : 21/04/2023
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

extern DMA_HandleTypeDef TOLOSAT_UART_TMTC_DMA_RX;
extern DMA_HandleTypeDef TOLOSAT_UART_TMTC_DMA_TX;


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
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN);
}

/**
 * @brief This function handles I2C1 event interrupt.
 */
void I2C1_EV_IRQHandler(void)
{
    HAL_I2C_EV_IRQHandler(&iic_avionic_inst.handle_struct);
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_pl_inst.handle_struct);
}

#if defined(STM32F411xE) || defined(STM32F103xB)
/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_tmtc_inst.handle_struct);
}
#elif defined(STM32H745xx)
/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart_tmtc_inst.handle_struct);
}
#else
#error "Board is not supported"
#endif

#if defined(STM32F411xE)
/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_RX);
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_TX);
}
#elif defined(STM32F103xB)
/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Channel4_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_TX);
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Channel5_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_RX);
}
#elif defined(STM32H745xx)
/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_RX);
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&TOLOSAT_UART_TMTC_DMA_TX);
}
#else
#error "Board is not supported"
#endif