/**
 * @file    drv_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for UART functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_uart.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void UartGenericIRQHandler(void *param);
static void UartGenericDMAIRQHandler(void *param);
static returnCode_t UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *const uart_conf);
static returnCode_t UartSetUpDMA(uartDesc_t *uart_desc, const uartConf_t *const uart_conf);
static returnCode_t UartCheckRX(uartDesc_t *uart_desc);
static returnCode_t UartCheckTX(uartDesc_t *uart_desc);
static returnCode_t UartStopRX(uartDesc_t *uart_desc);
static returnCode_t UartStopTX(uartDesc_t *uart_desc);
static returnCode_t UartStopRXTX(uartDesc_t *uart_desc);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
returnCode_t UartOpen(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_desc != NULL) && (uart_conf != NULL) && (uart_conf->baudrate != 0u))
    {
        // Setup UART
        uart_desc->handle_struct.Instance          = uart_conf->uart_ref;
        uart_desc->handle_struct.Init.BaudRate     = uart_conf->baudrate;
        uart_desc->handle_struct.Init.WordLength   = UART_WORDLENGTH_8B;
        uart_desc->handle_struct.Init.StopBits     = UART_STOPBITS_1;
        uart_desc->handle_struct.Init.Parity       = UART_PARITY_NONE;
        uart_desc->handle_struct.Init.Mode         = UART_MODE_TX_RX;
        uart_desc->handle_struct.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        uart_desc->handle_struct.Init.OverSampling = UART_OVERSAMPLING_16;

        // Init UART
        HAL_StatusTypeDef test_val = HAL_UART_Init(&uart_desc->handle_struct);
        if (test_val == HAL_OK)
        {
            // Set current mode
            uart_desc->current_mode = uart_conf->default_mode;
            // Setup DMA if necessary
            if (uart_conf->default_mode == DMA_MODE)
            {
                return_value = UartSetUpDMA(uart_desc, uart_conf);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Finally setup IRQ
                    return_value = UartSetupIRQs(uart_desc, uart_conf);
                }
            }
            else
            {
                // Finally setup IRQ
                return_value = UartSetupIRQs(uart_desc, uart_conf);
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          UartWrite(uartDesc_t *uart_desc, data_t data, length_t length)
 * @brief       Function that write over a UART connection
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to send
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
returnCode_t UartWrite(uartDesc_t *uart_desc, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_desc != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Write with driven mode
        if (uart_desc->current_mode == DMA_MODE)
        {
            test_val = HAL_UART_Transmit_DMA(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_UART_Transmit_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_Transmit(&uart_desc->handle_struct, data, length, DRV_MAX_DELAY);
        }
        else
        {
            test_val = HAL_ERROR;
        }

        // Check return value
        switch (test_val)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
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
 * @fn          UartRead(uartDesc_t *uart_desc, data_t data, length_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_desc   Instance that contains UART parameters and UART Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
returnCode_t UartRead(uartDesc_t *uart_desc, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_desc != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if (uart_desc->current_mode == DMA_MODE)
        {
            test_val = HAL_UARTEx_ReceiveToIdle_DMA(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_UARTEx_ReceiveToIdle_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            uint16_t nb_byte_received = 0;

            test_val = HAL_UARTEx_ReceiveToIdle(&uart_desc->handle_struct, data, length, &nb_byte_received, DRV_MAX_DELAY);
        }
        else
        {
            test_val = HAL_ERROR;
        }

        // Check return value
        switch (test_val)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT :
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY :
                return_value = RET_NOT_AVAILABLE;
                break;
            default :
                KernelPanic();
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
 * @fn              UartIoctl(uartDesc_t *uart_desc, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t UartIoctl(uartDesc_t *uart_desc, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused parameters
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        switch (cmd)
        {
            case IOCTL_PERIPHERAL_CHECK_RX :
                return_value = UartCheckRX(uart_desc);
                break;
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = UartCheckTX(uart_desc);
                break;
            case IOCTL_PERIPHERAL_STOP_RX :
                return_value = UartStopRX(uart_desc);
                break;
            case IOCTL_PERIPHERAL_STOP_TX :
                return_value = UartStopTX(uart_desc);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
                return_value = UartStopRXTX(uart_desc);
                break;
            default :
                return_value = RET_INVALID_PARAM;
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
 * @fn              UartClose(uartDesc_t *uart_desc)
 * @brief           Function that desinit the UART connection and puts defaults parameters
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase uart_desc
 */
returnCode_t UartClose(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        HAL_UART_DeInit(&uart_desc->handle_struct);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this UART
 */
static returnCode_t UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_conf->default_mode == INTERRUPT_MODE) || (uart_conf->default_mode == DMA_MODE))
    {
        // Set uart inst as the interrupt parameter to pass it to the interrupt routine
        IRQHandlerParam_t param = (IRQHandlerParam_t)uart_desc;
        // Request the interrupt
        return_value = RequestIRQ(uart_conf->irq_no, 5u, UartGenericIRQHandler, param);
    }

    return return_value;
}

/**
 * @fn              UartSetUpDMA(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if DMA is not available for this UART
 */
static returnCode_t UartSetUpDMA(uartDesc_t *uart_desc, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Check parameter(s)
    if (uart_conf->default_mode == DMA_MODE)
    {
        // First enable clock for DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();

        // Setup DMA RX
        uart_desc->dma_rx_handle_struct.Instance = uart_conf->dma_rx_ref;
#if defined(STM32H7)
        uart_desc->dma_rx_handle_struct.Init.Request = uart_conf->dma_rx_channel;
#elif defined(STM32F4)
        uart_desc->dma_rx_handle_struct.Init.Channel = uart_conf->dma_rx_channel;
#else
#error
#endif
        uart_desc->dma_rx_handle_struct.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        uart_desc->dma_rx_handle_struct.Init.PeriphInc           = DMA_PINC_DISABLE;
        uart_desc->dma_rx_handle_struct.Init.MemInc              = DMA_MINC_ENABLE;
        uart_desc->dma_rx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        uart_desc->dma_rx_handle_struct.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        uart_desc->dma_rx_handle_struct.Init.Mode                = DMA_NORMAL;
        uart_desc->dma_rx_handle_struct.Init.Priority            = DMA_PRIORITY_LOW;
        uart_desc->dma_rx_handle_struct.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        uart_desc->dma_rx_handle_struct.Parent                   = &uart_desc->handle_struct;
        uart_desc->handle_struct.hdmarx                          = &uart_desc->dma_rx_handle_struct;

        // Init DMA RX
        test_hal = HAL_DMA_Init(&uart_desc->dma_rx_handle_struct);
        if (test_hal == HAL_OK)
        {
            // Setup DMA TX
            uart_desc->dma_tx_handle_struct.Instance = uart_conf->dma_tx_ref;
#if defined(STM32H7)
            uart_desc->dma_tx_handle_struct.Init.Request = uart_conf->dma_tx_channel;
#elif defined(STM32F4)
            uart_desc->dma_tx_handle_struct.Init.Channel = uart_conf->dma_tx_channel;
#else
#error Architecture is not supported
#endif
            uart_desc->dma_tx_handle_struct.Init.Direction           = DMA_MEMORY_TO_PERIPH;
            uart_desc->dma_tx_handle_struct.Init.PeriphInc           = DMA_PINC_DISABLE;
            uart_desc->dma_tx_handle_struct.Init.MemInc              = DMA_MINC_ENABLE;
            uart_desc->dma_tx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            uart_desc->dma_tx_handle_struct.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
            uart_desc->dma_tx_handle_struct.Init.Mode                = DMA_NORMAL;
            uart_desc->dma_tx_handle_struct.Init.Priority            = DMA_PRIORITY_LOW;
            uart_desc->dma_tx_handle_struct.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
            uart_desc->dma_tx_handle_struct.Parent                   = &uart_desc->handle_struct;
            uart_desc->handle_struct.hdmatx                          = &uart_desc->dma_tx_handle_struct;

            // Init DMA TX
            test_hal = HAL_DMA_Init(&uart_desc->dma_tx_handle_struct);
            if (test_hal == HAL_OK)
            {
                // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)&uart_desc->dma_rx_handle_struct;
                // Request DMA RX interrupt
                return_value = RequestIRQ(uart_conf->dma_rx_irq_no, 8u, UartGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                    param = (IRQHandlerParam_t)&uart_desc->dma_tx_handle_struct;
                    // Request DMA TX interrupt
                    return_value = RequestIRQ(uart_conf->dma_tx_irq_no, 8u, UartGenericDMAIRQHandler, param);
                }
            }
            else
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartCheckRX(uartDesc_t *uart_desc)
 * @brief           Function that checks the status of a UART reception
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if UART is still receiving data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartCheckRX(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        if (uart_desc->handle_struct.RxState == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_desc->handle_struct.RxState == HAL_UART_STATE_BUSY_RX)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartCheckTX(uartDesc_t *uart_desc)
 * @brief           Function that checks the status of a UART trransmission
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if UART is still transfering data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartCheckTX(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        if (uart_desc->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_desc->handle_struct.gState == HAL_UART_STATE_BUSY_TX)
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartStopRX(uartDesc_t *uart_desc)
 * @brief           Function that stop the UART reception
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopRX(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_desc->current_mode == DMA_MODE) || (uart_desc->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_AbortReceive_IT(&uart_desc->handle_struct);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_AbortReceive(&uart_desc->handle_struct);
        }
        else
        {
            test_val = HAL_ERROR;
        }

        // Check return value
        switch (test_val)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            default :
                KernelPanic();
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
 * @fn              UartStopTX(uartDesc_t *uart_desc)
 * @brief           Function that stop the UART transmission
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopTX(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_desc->current_mode == DMA_MODE) || (uart_desc->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_AbortTransmit_IT(&uart_desc->handle_struct);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_AbortTransmit(&uart_desc->handle_struct);
        }
        else
        {
            test_val = HAL_ERROR;
        }

        // Check return value
        switch (test_val)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            default :
                KernelPanic();
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
 * @fn              UartStopRXTX(uartDesc_t *uart_desc)
 * @brief           Function that stop the UART reception and transmission
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopRXTX(uartDesc_t *uart_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_desc != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_desc->current_mode == DMA_MODE) || (uart_desc->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_Abort_IT(&uart_desc->handle_struct);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_Abort(&uart_desc->handle_struct);
        }
        else
        {
            test_val = HAL_ERROR;
        }

        // Check return value
        switch (test_val)
        {
            case HAL_OK :
                return_value = RET_SUCCESSFUL;
                break;
            default :
                KernelPanic();
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      UartGenericIRQHandler(void *param)
 * @brief   Generic UART IRQ Handler
 */
static void UartGenericIRQHandler(void *param)
{
    // Get uart inst
    uartDesc_t *uart_desc = (uartDesc_t *)param;

    // Save pre-interrupt status
    HAL_UART_StateTypeDef tx_status = uart_desc->handle_struct.gState;
    HAL_UART_StateTypeDef rx_status = uart_desc->handle_struct.RxState;

    // Do IRQ
    HAL_UART_IRQHandler(&uart_desc->handle_struct);

    // Check if something has changed
    if ((uart_desc->handle_struct.RxState != rx_status) && (uart_desc->handle_struct.RxState == HAL_UART_STATE_READY))
    {
        // RX completed
        if (uart_desc->callback_rx_completed != NULL)
        {
            uart_desc->callback_rx_completed(uart_desc->callback_rx_completed_param);
        }
    }
    if ((uart_desc->handle_struct.gState != tx_status) && (uart_desc->handle_struct.gState == HAL_UART_STATE_READY))
    {
        // TX completed
        if (uart_desc->callback_tx_completed != NULL)
        {
            uart_desc->callback_tx_completed(uart_desc->callback_tx_completed_param);
        }
    }
}

/**
 * @fn      UartGenericDMAIRQHandler(void *param)
 * @brief   Generic UART DMA IRQ Handler
 */
static void UartGenericDMAIRQHandler(void *param)
{
    DMAHandleStruct_t *handle_struct = (DMAHandleStruct_t *)param;
    HAL_DMA_IRQHandler(handle_struct);
}