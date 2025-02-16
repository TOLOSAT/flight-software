/**
 * @file    cmsdk_uart.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CMSDK UART functions
 * @date    09/06/2024
 *
 * Largely inspired by the Zephyr driver and STM32 HAL style.
 */

#ifndef CMSDK_UART_H
#define CMSDK_UART_H

/******************************* Include Files *******************************/

#include "cmsdk_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @brief Redefinition for compatibility purposes
 */
typedef CMSDK_UART_TypeDef UART_TypeDef;

/**
 * @brief Redefinition for compatibility purposes
 */
typedef CMSDK_UART_TypeDef USART_TypeDef;

/**
 * @brief  HAL Lock structures definition
 */
typedef enum
{
    HAL_UART_STATE_RESET   = 0u,
    HAL_UART_STATE_READY   = 1u,
    HAL_UART_STATE_BUSY_TX    = 2u,
    HAL_UART_STATE_BUSY_RX    = 3u,
    HAL_UART_STATE_ERROR   = 5u,
} HAL_UART_StateTypeDef;

/**
 * @struct  UART_HandleTypeDef
 * @brief   Struct type definition of a UART instance
 */
typedef struct
{
    UART_TypeDef *instance;
    uint32_t baud_rate;
    HAL_UART_StateTypeDef gstate;
    HAL_UART_StateTypeDef rxstate;
    uint8_t *p_tx_data;
    uint16_t tx_data_size;
    uint16_t tx_data_count;
    uint8_t *p_rx_data;
    uint16_t rx_data_size;
    uint16_t rx_data_count;
} UART_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

HAL_StatusTypeDef cmsdk_UartInit(UART_HandleTypeDef *uart);
HAL_StatusTypeDef cmsdk_UartTx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout);
HAL_StatusTypeDef cmsdk_UartTx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length);
HAL_StatusTypeDef cmsdk_UartRx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout);
HAL_StatusTypeDef cmsdk_UartRx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length);
HAL_StatusTypeDef cmsdk_UartDeInit(UART_HandleTypeDef *uart);
void cmsdk_UartRxIRQHandler(UART_HandleTypeDef *uart);
void cmsdk_UartTxIRQHandler(UART_HandleTypeDef *uart);

#endif /* CMSDK_UART_H */