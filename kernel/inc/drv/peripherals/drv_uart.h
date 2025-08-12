/**
 * @file    drv_uart.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for UART functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_uart UART Driver
 * @brief Abstraction layer for controlling UART buses.
 * @{
 */

#ifndef DRV_UART_H
#define DRV_UART_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief UART handle struct type redefinition */
typedef UART_HandleTypeDef uartHandleStruct_t;

/** @brief UART reference type redefinition (USART1, USART2, ...) */
typedef USART_TypeDef uartRef_t;

/** @brief UART baudrate type definition */
typedef uint32_t uartBaudRate_t;

/**
 * @struct  uartConf_t
 * @brief   Struct type definition of a UART configuration
 */
typedef struct
{
    /* UART Handle, Reference and Interrupt */
    uartRef_t *uart_ref; /**< @brief UART reference (USART1, USART2, ...) */
    IRQNo_t irq_no;      /**< @brief UART related interrupt */
    /* Configuration Parameters */
    drivingMode_t default_mode; /**< @brief UART driving mode */
    uartBaudRate_t baudrate;    /**< @brief UART instance baudrate */
    /* DMA */
    DMARef_t *dma_rx_ref;        /**< @brief DMA RX reference (DMA1_Stream0, ...) */
    DMARef_t *dma_tx_ref;        /**< @brief DMA TX reference (DMA1_Stream0, ...) */
    DMAChannel_t dma_rx_channel; /**< @brief DMA RX related channel */
    DMAChannel_t dma_tx_channel; /**< @brief DMA TX related channel */
    IRQNo_t dma_rx_irq_no;       /**< @brief DMA RX interrupt */
    IRQNo_t dma_tx_irq_no;       /**< @brief DMA TX interrupt */
} uartConf_t;

/**
 * @struct  uartDesc_t
 * @brief   Struct type definition of a UART descriptor
 */
typedef struct
{
    /* UART Handle, Reference and Interrupt */
    uartHandleStruct_t handle_struct; /**< @brief UART handle struct used by HAL */
    drivingMode_t current_mode;       /**< @brief Current driving mode */
    /* DMA */
    DMAHandleStruct_t dma_rx_handle_struct; /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct; /**< @brief DMA TX handle struct used by HAL */
    /* Callbacks */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
} uartDesc_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t UartOpen(uartDesc_t *uart_desc, const uartConf_t *const uart_conf);
extern returnCode_t UartWrite(uartDesc_t *uart_desc, data_t data, length_t length);
extern returnCode_t UartRead(uartDesc_t *uart_desc, data_t data, length_t length);
extern returnCode_t UartIoctl(uartDesc_t *uart_desc, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t UartClose(uartDesc_t *uart_desc);

#endif /* DRV_UART_H */

/**
 * @}
 * @}
 * @}
 */