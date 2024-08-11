/**
 * @file    generic_hal_uart.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL UART functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup generic_hal GENERIC HAL
 * @{
 * @defgroup drv_uart Driver UART
 * Functions that allows to drive UART independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef GENERIC_HAL_UART_H
#define GENERIC_HAL_UART_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"
#include "generic_hal_irq.h"

/***************************** Macros Definitions ****************************/

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

/** @brief UART message type definition */
typedef uint8_t uartMsg_t;

/** @brief UART message length type definition */
typedef uint16_t uartMsgLength_t;

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
 * @enum    uartIoCtlAction_t
 * @brief   UART IOCTL action type enum
 */
typedef enum
{
    UART_IOCTL_START_RX = 0u,   /**< UART IO CTL start DMA or IT tx */
    UART_IOCTL_START_TX,        /**< UART IO CTL start DMA or IT rx */
    UART_IOCTL_CHECK_RX_ENDED,  /**< UART IO CTL verify if DMA or IT rx ended */
    UART_IOCTL_CHECK_TX_ENDED,  /**< UART IO CTL verify if DMA or IT tx ended */
    UART_IOCTL_NB_ACTION,       /**< UART IO CTL maximum number of actions */
} uartIoCtlAction_t;

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

extern halStatus_t UartOpen(uartInst_t *uart_inst);
extern halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length);
extern halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length);
extern halStatus_t UartIoctl(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
extern halStatus_t UartClose(uartInst_t *uart_inst);

#endif /* GENERIC_HAL_UART_H */

/** 
 * @} 
 * @} 
 */