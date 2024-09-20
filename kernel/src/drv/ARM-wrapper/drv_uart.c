/**
 * @file    drv_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC UART functions
 * @date    04/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_uart.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #KERNEL_SUCCESSFUL if creation succeed
 * @retval          #KERNEL_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
kernelStatus_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (uart_inst->baudrate != 0u))
    {
        uart_inst->handle_struct.instance = uart_inst->uart_ref;
        uart_inst->handle_struct.baud_rate = uart_inst->baudrate;
        HAL_StatusTypeDef status = cmsdk_UartInit(&uart_inst->handle_struct);
        if (status != HAL_OK)
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to send
 * @retval      #KERNEL_SUCCESSFUL if message sent successfully
 * @retval      #KERNEL_INVALID_PARAM if one pointer is null
 * @retval      #KERNEL_TIMEOUT if uart timed out before sending message
 * @retval      #KERNEL_BUSY if uart is still sending previous message
 * @retval      #KERNEL_ERROR if transmit went wrong
 */
kernelStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef status = cmsdk_UartTx(&uart_inst->handle_struct, msg, length, DRV_MAX_DELAY);
        if (status != HAL_OK)
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #KERNEL_SUCCESSFUL if message sent successfully
 * @retval      #KERNEL_INVALID_PARAM if one pointer is null
 * @retval      #KERNEL_TIMEOUT if uart timed out before sending message
 * @retval      #KERNEL_BUSY if uart is still sending previous message
 * @retval      #KERNEL_ERROR if transmit went wrong
 */
kernelStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef status = cmsdk_UartRx(&uart_inst->handle_struct, msg, length, DRV_MAX_DELAY);
        if (status != HAL_OK)
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in]       cmd IO Control command
 * @param[in,out]   data IO Control command
 * @param[in]       data_size IO Control data size
 * @retval          #KERNEL_INVALID_PARAM if instance is a null pointer
 * @retval          #KERNEL_BUSY if action cannot be performed because driver is busy
 * @retval          #KERNEL_ERROR if io control encountered an error
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        (void)(uart_inst);
        (void)(cmd);
        (void)(data);
        (void)(data_size);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #KERNEL_SUCCESSFUL if changing parameters succeed
 * @retval          #KERNEL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase uart_inst
 */
kernelStatus_t UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        (void)(uart_inst);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}
