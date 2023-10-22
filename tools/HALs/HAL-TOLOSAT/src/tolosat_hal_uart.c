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
static halStatus_t UartDMAStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMAStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMACheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMACheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  #THAL_SUCCESSFUL if creation succeed
 * @retval  #THAL_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (uart_inst->baud_rate != 0u))
    {
        if ((uart_inst->uart_ref == TAPAS_UART_TMTC) || (uart_inst->uart_ref == TAPAS_UART_PRINT) || (uart_inst->uart_ref == TAPAS_UART_PL))
        {
            return_value = UartSetUpDMA(uart_inst);
            if (return_value == THAL_SUCCESSFUL)
            {
                uart_inst->handle_struct.Instance = uart_inst->uart_ref;
                uart_inst->handle_struct.Init.BaudRate = uart_inst->baud_rate;
                uart_inst->handle_struct.Init.WordLength = UART_WORDLENGTH_8B;
                uart_inst->handle_struct.Init.StopBits = UART_STOPBITS_1;
                uart_inst->handle_struct.Init.Parity = UART_PARITY_NONE;
                uart_inst->handle_struct.Init.Mode = UART_MODE_TX_RX;
                uart_inst->handle_struct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
                uart_inst->handle_struct.Init.OverSampling = UART_OVERSAMPLING_16;
                uint32_t test_val = HAL_UART_Init(&uart_inst->handle_struct);
                if (test_val != HAL_OK)
                {
                    return_value = THAL_ERROR;
                }
                else
                {
                    return_value = UartEnableInterrupt(uart_inst);
                }
            }
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to send
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if uart timed out before sending message
 * @retval      #THAL_BUSY if uart is still sending previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((uart_inst->drive_type == UART_POLLING_DRIVE) || (uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if (uart_inst->drive_type == UART_DMA_DRIVE)
            {
#if defined(CACHE_AVAILABLE)
                // Flush Cache into RAM in order to have the right data on RAM before using DMA
                SCB_CleanInvalidateDCache_by_Addr(msg, length);
#endif
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
                return_value = THAL_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = THAL_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = THAL_BUSY;
                break;
            default:
                return_value = THAL_ERROR;
                break;
            }
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if uart timed out before sending message
 * @retval      #THAL_BUSY if uart is still sending previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * NB : Only USART1 feature DMA on this TOLOSAT HAL
 */
halStatus_t UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((uart_inst->drive_type == UART_POLLING_DRIVE) || (uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
        {
            uint32_t test_val;
            // Read with driven mode
            if (uart_inst->drive_type == UART_DMA_DRIVE)
            {
#if defined(CACHE_AVAILABLE)
                // Flush Cache into RAM in order to have the right data on RAM before using DMA
                SCB_CleanInvalidateDCache_by_Addr(msg, length);
#endif
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
                return_value = THAL_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = THAL_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = THAL_BUSY;
                break;
            default:
                return_value = THAL_ERROR;
                break;
            }
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartIoctl(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if action cannot be performed because driver is busy
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t UartIoctl(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        switch (io_cmd.cmd)
        {
        case UART_IOCTL_DMA_START_RX:
            return_value = UartDMAStartRX(uart_inst, io_cmd);
            break;
        case UART_IOCTL_DMA_START_TX:
            return_value = UartDMAStartTX(uart_inst, io_cmd);
            break;
        case UART_IOCTL_DMA_CHECK_RX_ENDED:
            return_value = UartDMACheckRXEnded(uart_inst, io_cmd);
            break;
        case UART_IOCTL_DMA_CHECK_TX_ENDED:
            return_value = UartDMACheckTXEnded(uart_inst, io_cmd);
            break;
        default:
            return_value = THAL_INVALID_PARAM;
            break;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #THAL_SUCCESSFUL if changing parameters succeed
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase uart_inst
 */
halStatus_t UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
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
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartSetUpDMA(uartInst_t *uart_inst)
 * @brief       Function that setup DMA if it exists
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if DMA is not available for this UART
 */
static halStatus_t UartSetUpDMA(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (uart_inst->drive_type == UART_DMA_DRIVE)
    {
        if (uart_inst->uart_ref == TAPAS_UART_TMTC)
        {
            /* DMA controller clock enable */
            TAPAS_UART_TMTC_DMA_CLK_ENABLE();

            /* DMA interrupt init */
            /* TAPAS_UART_TMTC_DMA_RX_IRQ_NO interrupt configuration */
            HAL_NVIC_SetPriority(TAPAS_UART_TMTC_DMA_RX_IRQ_NO, 8, 0);
            HAL_NVIC_EnableIRQ(TAPAS_UART_TMTC_DMA_RX_IRQ_NO);
            /* TAPAS_UART_TMTC_DMA_TX_IRQ_NO interrupt configuration */
            HAL_NVIC_SetPriority(TAPAS_UART_TMTC_DMA_TX_IRQ_NO, 8, 0);
            HAL_NVIC_EnableIRQ(TAPAS_UART_TMTC_DMA_TX_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          UartEnableInterrupt(uartInst_t *uart_inst)
 * @brief       Function that enables interrupt if needed
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this UART
 */
static halStatus_t UartEnableInterrupt(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
    {
        if (uart_inst->uart_ref == TAPAS_UART_TMTC)
        {
            HAL_NVIC_SetPriority(TAPAS_UART_TMTC_IRQ_NO, 5, 0);
            HAL_NVIC_EnableIRQ(TAPAS_UART_TMTC_IRQ_NO);
        }
        else if (uart_inst->uart_ref == TAPAS_UART_PRINT)
        {
            HAL_NVIC_SetPriority(TAPAS_UART_PRINT_IRQ_NO, 5, 0);
            HAL_NVIC_EnableIRQ(TAPAS_UART_PRINT_IRQ_NO);
        }
        else if (uart_inst->uart_ref == TAPAS_UART_PL)
        {
            HAL_NVIC_SetPriority(TAPAS_UART_PL_IRQ_NO, 5, 0);
            HAL_NVIC_EnableIRQ(TAPAS_UART_PL_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          UartDisableInterrupt(uartInst_t *uart_inst)
 * @brief       Function that disables interrupt if needed
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this UART
 */
static halStatus_t UartDisableInterrupt(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
    {
        if (uart_inst->uart_ref == TAPAS_UART_TMTC)
        {
            HAL_NVIC_DisableIRQ(TAPAS_UART_TMTC_IRQ_NO);
        }
        else if (uart_inst->uart_ref == TAPAS_UART_PRINT)
        {
            HAL_NVIC_DisableIRQ(TAPAS_UART_PRINT_IRQ_NO);
        }
        else if (uart_inst->uart_ref == TAPAS_UART_PL)
        {
            HAL_NVIC_DisableIRQ(TAPAS_UART_PL_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn              UartDMAStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that starts DMA RX giving pointer to data to DMA
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 */
static halStatus_t UartDMAStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (io_cmd.data_size != 0u) && (io_cmd.data != NULL))
    {
        // Use Receive DMA to configure DMA (because it actually configures DMA in the first place)
        uint32_t test_val = HAL_UARTEx_ReceiveToIdle_DMA(&uart_inst->handle_struct, io_cmd.data, io_cmd.data_size);
        if (test_val != HAL_OK)
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMAStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that starts DMA TX giving pointer to data to DMA
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 */
static halStatus_t UartDMAStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (io_cmd.data_size != 0u) && (io_cmd.data != NULL))
    {
        // Currently ST UART DMA TX does not need anything
        (void)(uart_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMACheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that checks if DMA ended RX transfer
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if DMA is still receiving data
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 */
static halStatus_t UartDMACheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Unused Parameters
    (void)(io_cmd);

    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.gState == HAL_UART_STATE_BUSY_RX)
        {
            return_value = THAL_BUSY;
        }
        else if (uart_inst->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = THAL_SUCCESSFUL;
        }
        else
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMACheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that checks if DMA ended TX transfer
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if DMA is still transfering data
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 */
static halStatus_t UartDMACheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Unused Parameters
    (void)(io_cmd);

    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.gState == HAL_UART_STATE_BUSY_TX)
        {
            return_value = THAL_BUSY;
        }
        else if (uart_inst->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = THAL_SUCCESSFUL;
        }
        else
        {
            return_value = THAL_ERROR;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}