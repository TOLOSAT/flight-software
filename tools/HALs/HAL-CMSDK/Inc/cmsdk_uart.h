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
    HAL_UART_STATE_RESET   = 0u,    /**< @brief UART in reset state */
    HAL_UART_STATE_READY   = 1u,    /**< @brief UART is ready to transmit or receive */
    HAL_UART_STATE_BUSY_TX = 2u,    /**< @brief UART is busy transmitting */
    HAL_UART_STATE_BUSY_RX = 3u,    /**< @brief UART is busy receiving */
    HAL_UART_STATE_ERROR   = 5u,    /**< @brief UART has encountered an error needing a reset */
} HAL_UART_StateTypeDef;

/**
 * @struct  UART_HandleTypeDef
 * @brief   Struct type definition of a UART instance
 */
typedef struct
{
    UART_TypeDef *instance;         /**< @brief UART instance */
    uint32_t baud_rate;             /**< @brief UART baudrate */
    HAL_UART_StateTypeDef gstate;   /**< @brief UART global state (used also for transmitting state) */
    HAL_UART_StateTypeDef rxstate;  /**< @brief UART receive state */
    uint8_t *p_tx_data;             /**< @brief UART transmitting data pointer (used by interrupts) */
    uint16_t tx_data_size;          /**< @brief UART transmitting data size (used by interrupts) */
    uint16_t tx_data_count;         /**< @brief UART transmitting data counter (used by interrupts) */
    uint8_t *p_rx_data;             /**< @brief UART receiving data pointer (used by interrupts) */
    uint16_t rx_data_size;          /**< @brief UART receiving data size (used by interrupts) */
    uint16_t rx_data_count;         /**< @brief UART receiving data counter (used by interrupts) */
} UART_HandleTypeDef;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern HAL_StatusTypeDef cmsdk_UartInit(UART_HandleTypeDef *uart);
extern HAL_StatusTypeDef cmsdk_UartTx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout);
extern HAL_StatusTypeDef cmsdk_UartTx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length);
extern HAL_StatusTypeDef cmsdk_UartRx(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length, uint32_t timeout);
extern HAL_StatusTypeDef cmsdk_UartRx_IT(UART_HandleTypeDef *uart, uint8_t *msg, uint16_t length);
extern HAL_StatusTypeDef cmsdk_UartDeInit(UART_HandleTypeDef *uart);
extern void cmsdk_UartRxIRQHandler(UART_HandleTypeDef *uart);
extern void cmsdk_UartTxIRQHandler(UART_HandleTypeDef *uart);

#endif /* CMSDK_UART_H */