/**
 * @file tolosat_hal_uart.c
 * @author Merlin Kooshmanian
 * @brief Source file for TOLOSAT HAL UART functions
 * @date 30/04/2023
 *
 * Last Update : 30/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      UartOpen(uartInst_t *uart_inst)
 * @brief   Function that initialise a UART connection
 * @param   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  FCT_SUCCESSFUL if creation succeed
 * @retval  FCT_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
halStatus_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    // Function Core
    if(uart_inst != NULL && &uart_inst->handle_struct != NULL && uart_inst->baud_rate != 0)
    {
#if defined(STM32F411xE)
        if(uart_inst->uart_ref == USART1 || uart_inst->uart_ref == USART2 || uart_inst->uart_ref == USART6)
#endif
#if defined(STM32F103xB)
        if(uart_inst->uart_ref == USART1 || uart_inst->uart_ref == USART2 || uart_inst->uart_ref == USART3)
#endif
        {
            uart_inst->handle_struct.Instance = uart_inst->uart_ref;
            uart_inst->handle_struct.Init.BaudRate = uart_inst->baud_rate;
            uart_inst->handle_struct.Init.WordLength = UART_WORDLENGTH_8B;
            uart_inst->handle_struct.Init.StopBits = UART_STOPBITS_1;
            uart_inst->handle_struct.Init.Parity = UART_PARITY_NONE;
            uart_inst->handle_struct.Init.Mode = UART_MODE_TX_RX;
            uart_inst->handle_struct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
            uart_inst->handle_struct.Init.OverSampling = UART_OVERSAMPLING_16;
            test_val = HAL_UART_Init(&uart_inst->handle_struct);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

/**
 * @fn      UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief   Function that write over a UART connection
 * @param   uart_inst Instance that contains UART parameters and UART Handler
 * @param   msg Message we want to send
 * @param   length Size of the message we want to send 
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 * 
 * Attention : currently works only in polling mode
 * Needs to supports IT and DMA
 */
halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    // Function Core
    if(uart_inst != NULL && msg != NULL && length != 0)
    {
        if(uart_inst->drive_type == UART_POLLING_DRIVE)
        {
            test_val = HAL_UART_Transmit(&uart_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

/**
 * @fn      UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief   Function that read over UART connection
 * @param   uart_inst Instance that contains UART parameters and UART Handler
 * @param   msg Message we want to receive
 * @param   length Size of the message we want to receive 
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 * 
 * Attention : currently works only in polling mode
 * Needs to supports IT and DMA
 */
halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    // Function Core
    if(uart_inst != NULL && msg != NULL && length != 0)
    {
        if(uart_inst->drive_type == UART_POLLING_DRIVE)
        {
            test_val = HAL_UART_Receive(&uart_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }


    return(return_value);
}

/**
 * @fn      UartIoctl(uartInst_t *uart_inst)
 * @brief   Function that allows to change parameters such as drive mode, baudrate etc
 * @param   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 * 
 * This feature is not supported yet so it does nothing
 */
halStatus_t UartIoctl(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(uart_inst != NULL)
    {
        /* TO DO */
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }


    return(return_value);
}

/**
 * @fn      UartClose(uartInst_t *uart_inst)
 * @brief   Function that desinit the UART connection and puts defaults parameters
 * @param   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 * 
 * This function erase uart_inst
 */
halStatus_t UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uartInst_t null_inst = {
        .handle_struct = {0},
        .drive_type = 0,
        .uart_ref = 0,
        .baud_rate = 0,
    };

    // Function Core
    if(uart_inst != NULL)
    {
        HAL_UART_DeInit(&uart_inst->handle_struct);
        *uart_inst = null_inst;
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}
