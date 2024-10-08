/**
 * @file    drv_uart.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for UART functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

#include "drv/drv_types.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

#define UART_IOCTL_START_RX         0u  /**< UART IO CTL start DMA or IT tx */
#define UART_IOCTL_START_TX         1u  /**< UART IO CTL start DMA or IT rx */
#define UART_IOCTL_CHECK_RX_ENDED   2u  /**< UART IO CTL verify if DMA or IT rx ended */
#define UART_IOCTL_CHECK_TX_ENDED   3u  /**< UART IO CTL verify if DMA or IT tx ended */

/***************************** Types Definitions *****************************/

/** @brief UART handle struct type redefinition */
typedef UART_HandleTypeDef uartHandleStruct_t;

/** @brief UART DMA handle struct type redefinition */
typedef DMA_HandleTypeDef uartDMAHandleStruct_t;

/** @brief UART reference type redefinition (USART1, USART2, ...) */
typedef USART_TypeDef uartRef_t;

/** @brief UART DMA reference type redefinition (DMA1_Stream0, DMA1_Stream1, ...) */
typedef DMA_Stream_TypeDef uartDMARef_t;

/** @brief UART baud rate type definition */
typedef uint32_t uartBaudRate_t;

/** 
 * @enum    uartDriveType_t
 * @brief   UART driving mode type enum
 */
typedef enum
{
    UART_POLLING_DRIVE = 0u,   /**< UART is driven in polling mode (CPU waits the data) */
    UART_INTERRUPT_DRIVE = 1u, /**< UART is driven by interrupts (CPU interrupts when there is data) */
    UART_DMA_DRIVE = 2u,       /**< UART is driven by DMA (when there is data DMA puts it in RAM without CPU call) */
} uartDriveType_t;

/** 
 * @struct  uartInst_t
 * @brief   Struct type definition of a UART instance
 */
typedef struct
{
    uartHandleStruct_t handle_struct;           /**< @brief UART handle struct used by HAL */
    uartDMAHandleStruct_t dma_rx_handle_struct; /**< @brief UART DMA RX handle struct used by HAL */
    uartDMAHandleStruct_t dma_tx_handle_struct; /**< @brief UART DMA TX handle struct used by HAL */
    uartRef_t *uart_ref;                        /**< @brief UART reference (USART1, USART2, ...) */
    uartDMARef_t *dma_rx_ref;                   /**< @brief UART DMA RX reference (DMA1_Stream0, DMA1_Stream0, ...) */
    uartDMARef_t *dma_tx_ref;                   /**< @brief UART DMA TX reference (DMA1_Stream0, DMA1_Stream0, ...) */
    uartDriveType_t drive_type;                 /**< @brief UART drive mode as defining in uartDriveType_t enum */
    uartBaudRate_t baudrate;                    /**< @brief UART instance baudrate */
    IRQNo_t irq_no;                             /**< @brief UART related interrupt (IRQ_NONE if none) */
    IRQNo_t dma_rx_irq_no;                      /**< @brief UART DMA RX related interrupt (IRQ_NONE if none) */
    IRQNo_t dma_tx_irq_no;                      /**< @brief UART DMA TX related interrupt (IRQ_NONE if none) */
} uartInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t UartOpen(uartInst_t *uart_inst);
extern returnCode_t UartWrite(uartInst_t *uart_inst, data_t data, length_t length);
extern returnCode_t UartRead(uartInst_t *uart_inst, data_t data, length_t length);
extern returnCode_t UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t UartClose(uartInst_t *uart_inst);

#endif /* DRV_UART_H */

/** 
 * @}
 * @}
 * @}
 */