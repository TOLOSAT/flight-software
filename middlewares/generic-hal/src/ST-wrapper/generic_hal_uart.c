/**
 * @file    generic_hal_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC HAL UART functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void UartGenericIRQHandler(void *param);
static halStatus_t UartSetUpDMA(const uartInst_t *uart_inst);
static halStatus_t UartSetupIRQs(const uartInst_t *uart_inst);
static halStatus_t UartDMAorITStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMAorITStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMAorITCheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
static halStatus_t UartDMAorITCheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval  #GEN_HAL_SUCCESSFUL if creation succeed
 * @retval  #GEN_HAL_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 *
 * NB : Only UART_TMTC feature DMA on this GENERIC HAL
 */
halStatus_t IN_UART_TEXT_SECTION UartOpen(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (uart_inst->baudrate != 0u))
    {
        return_value = UartSetUpDMA(uart_inst);
        if (return_value == GEN_HAL_SUCCESSFUL)
        {
            uart_inst->handle_struct.Instance = uart_inst->uart_ref;
            uart_inst->handle_struct.Init.BaudRate = uart_inst->baudrate;
            uart_inst->handle_struct.Init.WordLength = UART_WORDLENGTH_8B;
            uart_inst->handle_struct.Init.StopBits = UART_STOPBITS_1;
            uart_inst->handle_struct.Init.Parity = UART_PARITY_NONE;
            uart_inst->handle_struct.Init.Mode = UART_MODE_TX_RX;
            uart_inst->handle_struct.Init.HwFlowCtl = UART_HWCONTROL_NONE;
            uart_inst->handle_struct.Init.OverSampling = UART_OVERSAMPLING_16;
            uint32_t test_val = HAL_UART_Init(&uart_inst->handle_struct);
            if (test_val != HAL_OK)
            {
                return_value = GEN_HAL_ERROR;
            }
            else
            {
                return_value = UartSetupIRQs(uart_inst);
            }
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that write over a UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to send
 * @retval      #GEN_HAL_SUCCESSFUL if message sent successfully
 * @retval      #GEN_HAL_INVALID_PARAM if one pointer is null
 * @retval      #GEN_HAL_TIMEOUT if uart timed out before sending message
 * @retval      #GEN_HAL_BUSY if uart is still sending previous message
 * @retval      #GEN_HAL_ERROR if transmit went wrong
 *
 * NB : Only UART_TMTC feature DMA on this GENERIC HAL
 */
halStatus_t IN_UART_TEXT_SECTION UartWrite(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

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
                test_val = HAL_UART_Transmit(&uart_inst->handle_struct, msg, length, GENERIC_HAL_MAX_DELAY);
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = GEN_HAL_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = GEN_HAL_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = GEN_HAL_BUSY;
                break;
            default:
                return_value = GEN_HAL_ERROR;
                break;
            }
        }
        else
        {
            return_value = GEN_HAL_INVALID_PARAM;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #GEN_HAL_SUCCESSFUL if message sent successfully
 * @retval      #GEN_HAL_INVALID_PARAM if one pointer is null
 * @retval      #GEN_HAL_TIMEOUT if uart timed out before sending message
 * @retval      #GEN_HAL_BUSY if uart is still sending previous message
 * @retval      #GEN_HAL_ERROR if transmit went wrong
 *
 * NB : Only UART_TMTC feature DMA on this GENERIC HAL
 */
halStatus_t IN_UART_TEXT_SECTION UartRead(uartInst_t *uart_inst, uartMsg_t *msg, uartMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

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
                test_val = HAL_UARTEx_ReceiveToIdle_IT(&uart_inst->handle_struct, msg, length);
            }
            else
            {
                uint16_t nb_byte_received = 0;
                test_val = HAL_UARTEx_ReceiveToIdle(&uart_inst->handle_struct, msg, length, &nb_byte_received, GENERIC_HAL_MAX_DELAY);
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = GEN_HAL_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = GEN_HAL_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = GEN_HAL_BUSY;
                break;
            default:
                return_value = GEN_HAL_ERROR;
                break;
            }
        }
        else
        {
            return_value = GEN_HAL_INVALID_PARAM;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartIoctl(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_BUSY if action cannot be performed because driver is busy
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
halStatus_t IN_UART_TEXT_SECTION UartIoctl(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        switch (io_cmd.cmd)
        {
        case UART_IOCTL_START_RX:
            return_value = UartDMAorITStartRX(uart_inst, io_cmd);
            break;
        case UART_IOCTL_START_TX:
            return_value = UartDMAorITStartTX(uart_inst, io_cmd);
            break;
        case UART_IOCTL_CHECK_RX_ENDED:
            return_value = UartDMAorITCheckRXEnded(uart_inst, io_cmd);
            break;
        case UART_IOCTL_CHECK_TX_ENDED:
            return_value = UartDMAorITCheckTXEnded(uart_inst, io_cmd);
            break;
        default:
            return_value = GEN_HAL_INVALID_PARAM;
            break;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval          #GEN_HAL_SUCCESSFUL if changing parameters succeed
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase uart_inst
 */
halStatus_t IN_UART_TEXT_SECTION UartClose(uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        HAL_UART_DeInit(&uart_inst->handle_struct);
        return_value = DisableIRQ(uart_inst->irq_no);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartSetUpDMA(uartInst_t *uart_inst)
 * @brief       Function that setup DMA if it exists
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #GEN_HAL_SUCCESSFUL if changing parameters succeed
 * @retval      #GEN_HAL_INVALID_PARAM if DMA is not available for this UART
 */
static halStatus_t IN_UART_TEXT_SECTION UartSetUpDMA(const uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst->drive_type == UART_DMA_DRIVE)
    {
        if (uart_inst->uart_ref == UART_TMTC)
        {
            /* DMA controller clock enable */
            UART_TMTC_DMA_CLK_ENABLE();

            /* DMA interrupt init */
            /* UART_TMTC_DMA_RX_IRQ_NO interrupt configuration */
            HAL_NVIC_SetPriority(UART_TMTC_DMA_RX_IRQ_NO, 8, 0);
            HAL_NVIC_EnableIRQ(UART_TMTC_DMA_RX_IRQ_NO);
            /* UART_TMTC_DMA_TX_IRQ_NO interrupt configuration */
            HAL_NVIC_SetPriority(UART_TMTC_DMA_TX_IRQ_NO, 8, 0);
            HAL_NVIC_EnableIRQ(UART_TMTC_DMA_TX_IRQ_NO);
        }
        else
        {
            return_value = GEN_HAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          UartSetupIRQs(uartInst_t *uart_inst)
 * @brief       Function that setups interrupt if needed
 * @param[in]   uart_inst Instance that contains UART parameters and UART Handler
 * @retval      #GEN_HAL_SUCCESSFUL if changing parameters succeed
 * @retval      #GEN_HAL_INVALID_PARAM if IT is not available for this UART
 */
static halStatus_t IN_UART_TEXT_SECTION UartSetupIRQs(const uartInst_t *uart_inst)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst->drive_type == UART_INTERRUPT_DRIVE) || (uart_inst->drive_type == UART_DMA_DRIVE))
    {
        IRQHandlerParam_t param = (IRQHandlerParam_t)&uart_inst->handle_struct;
        return_value = RequestIRQ(uart_inst->irq_no, 5u, UartGenericIRQHandler, param);
    }

    return return_value;
}

/**
 * @fn              UartDMAorITStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that starts DMA RX giving pointer to data to DMA
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t IN_UART_TEXT_SECTION UartDMAorITStartRX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (io_cmd.data_size != 0u) && (io_cmd.data != NULL))
    {
        // First abort transfer if there is a previous one
        uint32_t test_val = HAL_UART_AbortReceive_IT(&uart_inst->handle_struct);
        if (test_val == HAL_OK)
        {
            if (uart_inst->drive_type == UART_DMA_DRIVE)
            {
                // Use Receive DMA to configure DMA (because it actually configures DMA in the first place)
                test_val = HAL_UARTEx_ReceiveToIdle_DMA(&uart_inst->handle_struct, io_cmd.data, io_cmd.data_size);
                if (test_val != HAL_OK)
                {
                    return_value = GEN_HAL_ERROR;
                }
            }
            else
            {
                // Use Receive IT to configure IT (because it actually configures IT in the first place)
                test_val = HAL_UARTEx_ReceiveToIdle_IT(&uart_inst->handle_struct, io_cmd.data, io_cmd.data_size);
                if (test_val != HAL_OK)
                {
                    return_value = GEN_HAL_ERROR;
                }
            }
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMAorITStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that starts DMA TX giving pointer to data to DMA
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t IN_UART_TEXT_SECTION UartDMAorITStartTX(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((uart_inst != NULL) && (io_cmd.data_size != 0u) && (io_cmd.data != NULL))
    {
        // Currently ST UART DMA TX or IT TX does not need anything
        (void)(uart_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMAorITCheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that checks if DMA ended RX transfer
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_BUSY if DMA is still receiving data
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t IN_UART_TEXT_SECTION UartDMAorITCheckRXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Unused Parameters
    (void)(io_cmd);

    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.gState == HAL_UART_STATE_BUSY_RX)
        {
            return_value = GEN_HAL_BUSY;
        }
        else if (uart_inst->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = GEN_HAL_SUCCESSFUL;
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartDMAorITCheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that checks if DMA ended TX transfer
 * @param[in,out]   uart_inst Instance that contains UART parameters and UART Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #GEN_HAL_INVALID_PARAM if instance is a null pointer
 * @retval          #GEN_HAL_BUSY if DMA is still transfering data
 * @retval          #GEN_HAL_ERROR if io control encountered an error
 * @retval          #GEN_HAL_SUCCESSFUL else
 */
static halStatus_t IN_UART_TEXT_SECTION UartDMAorITCheckTXEnded(uartInst_t *uart_inst, halIoCtlCmd_t io_cmd)
{
    // Unused Parameters
    (void)(io_cmd);

    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.gState == HAL_UART_STATE_BUSY_TX)
        {
            return_value = GEN_HAL_BUSY;
        }
        else if (uart_inst->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = GEN_HAL_SUCCESSFUL;
        }
        else
        {
            return_value = GEN_HAL_ERROR;
        }
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              UartGenericIRQHandler(void *param)
 * @brief           Generic UART Handler
 */
static void IN_UART_TEXT_SECTION UartGenericIRQHandler(void *param)
{
    UART_HandleTypeDef *handle_struct = (UART_HandleTypeDef *)param;
    HAL_UART_IRQHandler(handle_struct);
}