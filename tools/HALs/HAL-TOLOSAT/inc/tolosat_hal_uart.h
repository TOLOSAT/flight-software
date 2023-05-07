/**
 * @file tolosat_hal_uart.h
 * @author Merlin Kooshmanian
 * @brief Header file for TOLOSAT HAL UART functions
 * @date 30/04/2023
 *
 * Last Update : 30/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup drv_uart Driver UART TOLOSAT
 * Function that allows to drive UART independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef TOLOSAT_HAL_UART_H
#define TOLOSAT_HAL_UART_H

/***************************** Include Files *********************************/

#include "tolosat_hal_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef UART_HandleTypeDef uartHandleStruct_t;
typedef USART_TypeDef uartRef_t;
typedef uint32_t uartBaudRate_t;
typedef uint8_t uartMsg_t;
typedef uint16_t uartMsgLength_t;

typedef enum
{
    UART_POLLING_DRIVE = 0u,   /**< UART is driven in polling mode (CPU waits the data) */
    UART_INTERRUPT_DRIVE = 1u, /**< UART is driven by interrupts (CPU interrupts when there is data) */
    UART_DMA_DRIVE = 2u,       /**< UART is driven by DMA (when there is data DMA puts it in RAM without CPU call) */
} uartDriveType_t;

typedef struct
{
    uartHandleStruct_t handle_struct;
    uartRef_t *uart_ref;
    uartDriveType_t drive_type;
    uartBaudRate_t baud_rate;
} uartInst_t;

/************************** Function Prototypes ******************************/

halStatus_t UartOpen(uartInst_t *uart_inst);
halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length);
halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length);
halStatus_t UartIoctl(uartInst_t *uart_inst);
halStatus_t UartClose(uartInst_t *uart_inst);

#endif /* TOLOSAT_HAL_UART_H */

/** @} */