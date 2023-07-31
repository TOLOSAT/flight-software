/**
 * @file    tolosat_hal_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL UART functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t UartSetUpDMA(uartInst_t *uart_inst);
static halStatus_t UartEnableInterrupt(uartInst_t *uart_inst);
static halStatus_t UartDisableInterrupt(uartInst_t *uart_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  #FCT_SUCCESSFUL if creation succeed
 * @retval  #FCT_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val;

    // Function Core
    if ((uart_inst != NULL) && (uart_inst->baud_rate != 0u))
    {
#if defined(STM32F411xE)
        if ((uart_inst->uart_ref == USART1) || (uart_inst->uart_ref == USART2) || (uart_inst->uart_ref == USART6))
#elif defined(STM32F103xB)
        if ((uart_inst->uart_ref == USART1) || (uart_inst->uart_ref == USART2) || (uart_inst->uart_ref == USART3))
#elif defined(STM32H745xx)
        if ((uart_inst->uart_ref == USART1) || (uart_inst->uart_ref == USART2) || (uart_inst->uart_ref == USART3) || (uart_inst->uart_ref == USART6))
#else
#error "Board is not supported"
#endif
        {
            return_value = UartSetUpDMA(uart_inst);
            if (return_value == FCT_SUCCESSFUL)
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
                else
                {
                    return_value = UartEnableInterrupt(uart_inst);
                }
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

    return return_value;
}

/**
 * @fn          UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to send
 * @retval      #FCT_SUCCESSFUL if message sent successfully
 * @retval      #FCT_INVALID_PARAM if one pointer is null
 * @retval      #FCT_TIMEOUT if uart timed out before sending message
 * @retval      #FCT_BUSY if uart is still sending previous message
 * @retval      #FCT_ERROR if transmit went wrong
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((uart_inst->drive_type == UART_POLLING_DRIVE) || (uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if (uart_inst->drive_type == UART_DMA_DRIVE)
            {
                test_val = HAL_UART_Transmit_DMA(&uart_inst->handle_struct, msg, length);
            }
            else if (uart_inst->drive_type == UART_INTERRUPT_DRIVE)
            {
                test_val = HAL_UART_Transmit_IT(&uart_inst->handle_struct, msg, length);
            }
            else
            {
                test_val = HAL_UART_Transmit(&uart_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = FCT_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = FCT_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = FCT_BUSY;
                break;
            default:
                return_value = FCT_ERROR;
                break;
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

    return return_value;
}

/**
 * @fn          UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #FCT_SUCCESSFUL if message sent successfully
 * @retval      #FCT_INVALID_PARAM if one pointer is null
 * @retval      #FCT_TIMEOUT if uart timed out before sending message
 * @retval      #FCT_BUSY if uart is still sending previous message
 * @retval      #FCT_ERROR if transmit went wrong
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((uart_inst->drive_type == UART_POLLING_DRIVE) || (uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
        {
            uint32_t test_val;
            // Read with driven mode
            if (uart_inst->drive_type == UART_DMA_DRIVE)
            {
                test_val = HAL_UARTEx_ReceiveToIdle_DMA(&uart_inst->handle_struct, msg, length);
            }
            else if (uart_inst->drive_type == UART_INTERRUPT_DRIVE)
            {
                test_val = HAL_UART_Receive_IT(&uart_inst->handle_struct, msg, length);
            }
            else
            {
                test_val = HAL_UART_Receive(&uart_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = FCT_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = FCT_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = FCT_BUSY;
                break;
            default:
                return_value = FCT_ERROR;
                break;
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

    return return_value;
}

// cppcheck-suppress constParameter
/**
 * @fn              UartIoctl(uartInst_t *uart_inst)
 * @brief           Function that allows to change parameters such as drive mode, baudrate etc
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #FCT_SUCCESSFUL if changing parameters succeed
 * @retval          #FCT_INVALID_PARAM if instance is a null pointer
 *
 * @warning This feature is not supported yet so it does nothing
 * @todo Function may modifiy uart_inst handle_struct or baud_rate
 */
halStatus_t UartIoctl(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        /* TO DO */
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #FCT_SUCCESSFUL if changing parameters succeed
 * @retval          #FCT_INVALID_PARAM if instance is a null pointer
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
    if (uart_inst != NULL)
    {
        HAL_UART_DeInit(&uart_inst->handle_struct);
        return_value = UartDisableInterrupt(uart_inst);
        *uart_inst = null_inst;
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartSetUpDMA(uartInst_t *uart_inst)
 * @brief       Function that setup DMA if it exists
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #FCT_SUCCESSFUL if changing parameters succeed
 * @retval      #FCT_INVALID_PARAM if DMA is not available for this UART
 */
static halStatus_t UartSetUpDMA(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (uart_inst->drive_type == UART_DMA_DRIVE)
    {
#if defined(STM32F411xE)
        if (uart_inst->uart_ref == USART1)
        {
            /* DMA controller clock enable */
            __HAL_RCC_DMA2_CLK_ENABLE();

            /* DMA interrupt init */
            /* DMA2_Stream2_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
            /* DMA2_Stream7_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
        }
#elif defined(STM32F103xB)
        if (uart_inst->uart_ref == USART1)
        {
            /* DMA controller clock enable */
            __HAL_RCC_DMA1_CLK_ENABLE();

            /* DMA interrupt init */
            /* DMA1_Channel4_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
            /* DMA1_Channel5_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
        }
#elif defined(STM32H745xx)
        if (uart_inst->uart_ref == USART2)
        {
            /* DMA controller clock enable */
            __HAL_RCC_DMA1_CLK_ENABLE();

            /* DMA interrupt init */
            /* DMA1_Stream0_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
            /* DMA1_Stream1_IRQn interrupt configuration */
            HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 8, 0);
            HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
        }
#else
#error "Board is not supported"
#endif
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          UartEnableInterrupt(uartInst_t *uart_inst)
 * @brief       Function that enables interrupt if needed
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #FCT_SUCCESSFUL if changing parameters succeed
 * @retval      #FCT_INVALID_PARAM if IT is not available for this UART
 */
static halStatus_t UartEnableInterrupt(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if ((uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
    {
        if (uart_inst->uart_ref == USART1)
        {
            HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(USART1_IRQn);
        }
        else if (uart_inst->uart_ref == USART2)
        {
            HAL_NVIC_SetPriority(USART2_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(USART2_IRQn);
        }
#if defined(STM32F411xE) || defined(STM32H745xx)
        else if (uart_inst->uart_ref == USART6)
        {
            HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(USART6_IRQn);
        }
#endif
#if defined(STM32F103xB) || defined(STM32H745xx)
        else if (uart_inst->uart_ref == USART3)
        {
            HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(USART3_IRQn);
        }
#endif
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          UartDisableInterrupt(uartInst_t *uart_inst)
 * @brief       Function that disables interrupt if needed
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #FCT_SUCCESSFUL if changing parameters succeed
 * @retval      #FCT_INVALID_PARAM if IT is not available for this UART
 */
static halStatus_t UartDisableInterrupt(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if ((uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
    {
        if (uart_inst->uart_ref == USART1)
        {
            HAL_NVIC_DisableIRQ(USART1_IRQn);
        }
        else if (uart_inst->uart_ref == USART2)
        {
            HAL_NVIC_DisableIRQ(USART2_IRQn);
        }
#if defined(STM32F411xE) || defined(STM32H745xx)
        else if (uart_inst->uart_ref == USART6)
        {
            HAL_NVIC_DisableIRQ(USART6_IRQn);
        }
#endif
#if defined(STM32F103xB) || defined(STM32H745xx)
        else if (uart_inst->uart_ref == USART3)
        {
            HAL_NVIC_DisableIRQ(USART3_IRQn);
        }
#endif
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }

    return return_value;
}