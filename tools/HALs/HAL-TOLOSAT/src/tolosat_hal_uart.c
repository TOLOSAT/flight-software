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

halStatus_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    if(uart_inst != NULL && &uart_inst->handle_struct != NULL && uart_inst->baud_rate != 0)
    {
#if defined(STM32F411xE)
        if(uart_inst->uart_ref == USART1 || uart_inst->uart_ref == USART2 || uart_inst->uart_ref == USART6)
#endif
#if defined(STM32F103xB)
        if(uart_inst->uart_ref == USART1 || uart_inst->uart_ref == USART2 || uart_inst->uart_ref == USART3)
#endif
        {
            // Function Core
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

halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core


    return(return_value);
}

halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core


    return(return_value);
}

halStatus_t UartIoctl(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core


    return(return_value);
}

halStatus_t UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core


    return(return_value);
}
