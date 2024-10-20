/**
 * @file    drv_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC UART functions
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
 * @param[in,out]   uart_inst   Instance that contains UART parameters and UART Handler
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
returnCode_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (uart_inst->baudrate != 0u))
    {
        uart_inst->handle_struct.instance = uart_inst->uart_ref;
        uart_inst->handle_struct.baud_rate = uart_inst->baudrate;
        HAL_StatusTypeDef status = cmsdk_UartInit(&uart_inst->handle_struct);
        // Check return value
        switch (status)
        {
        case HAL_OK:
            return_value = RET_SUCCESSFUL;
            break;
        case HAL_TIMEOUT:
            return_value = RET_TIMEOUT;
            break;
        case HAL_BUSY:
            return_value = RET_NOT_AVAILABLE;
            break;
        default:
            return_value = RET_ERROR;
            break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartWrite(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to send
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 * @retval      #RET_ERROR if transmit went wrong
 */
returnCode_t UartWrite(uartInst_t *uart_inst, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef status = cmsdk_UartTx(&uart_inst->handle_struct, data, length, DRV_MAX_DELAY);
        // Check return value
        switch (status)
        {
        case HAL_OK:
            return_value = RET_SUCCESSFUL;
            break;
        case HAL_TIMEOUT:
            return_value = RET_TIMEOUT;
            break;
        case HAL_BUSY:
            return_value = RET_NOT_AVAILABLE;
            break;
        default:
            return_value = RET_ERROR;
            break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartRead(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 * @retval      #RET_ERROR if transmit went wrong
 */
returnCode_t UartRead(uartInst_t *uart_inst, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef status = cmsdk_UartRx(&uart_inst->handle_struct, data, length, DRV_MAX_DELAY);
        // Check return value
        switch (status)
        {
        case HAL_OK:
            return_value = RET_SUCCESSFUL;
            break;
        case HAL_TIMEOUT:
            return_value = RET_TIMEOUT;
            break;
        case HAL_BUSY:
            return_value = RET_NOT_AVAILABLE;
            break;
        default:
            return_value = RET_ERROR;
            break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_ERROR if io control encountered an error
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

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
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst   Instance that contains UART parameters and UART Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase uart_inst
 */
returnCode_t UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        (void)(uart_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
