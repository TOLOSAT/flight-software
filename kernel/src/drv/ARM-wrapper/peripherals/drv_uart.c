/**
 * @file    drv_uart.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC UART functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_uart.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void UartRxGenericIRQHandler(void *param);
static void UartTxGenericIRQHandler(void *param);
static returnCode_t UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *uart_conf);
static returnCode_t UartCheckRX(uartDesc_t *uart_desc);
static returnCode_t UartCheckTX(uartDesc_t *uart_desc);

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
    if ((uart_desc != NULL) && (uart_conf->baudrate != 0u))
    {
        // Setup UART
        uart_desc->handle_struct.instance  = uart_conf->uart_ref;
        uart_desc->handle_struct.baud_rate = uart_conf->baudrate;

        // Init UART
        HAL_StatusTypeDef status = cmsdk_UartInit(&uart_desc->handle_struct);
        if (status == HAL_OK)
        {
            // Set current mode
            uart_desc->current_mode = uart_conf->default_mode;
            // Then setup IRQ
            return_value = UartSetupIRQs(uart_desc, uart_conf);
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
 * @param[in]   uart_desc   Instance that contains UART parameters and UART Handler
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
        HAL_StatusTypeDef status = HAL_OK;
        // Write with driven mode
        if (uart_desc->current_mode == DMA_MODE)
        {
            status = cmsdk_UartTx_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == INTERRUPT_MODE)
        {
            status = cmsdk_UartTx_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            status = cmsdk_UartTx(&uart_desc->handle_struct, data, length, DRV_MAX_DELAY);
        }
        else
        {
            status = HAL_ERROR;
        }

        // Check return value
        switch (status)
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
        HAL_StatusTypeDef status = HAL_OK;
        // Read with driven mode
        if (uart_desc->current_mode == DMA_MODE)
        {
            status = cmsdk_UartRx_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == INTERRUPT_MODE)
        {
            status = cmsdk_UartRx_IT(&uart_desc->handle_struct, data, length);
        }
        else if (uart_desc->current_mode == POLLING_MODE)
        {
            status = cmsdk_UartRx(&uart_desc->handle_struct, data, length, DRV_MAX_DELAY);
        }
        else
        {
            status = HAL_ERROR;
        }

        // Check return value
        switch (status)
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
 * @param[in,out]   uart_desc   Instance that contains UART parameters and UART Handler
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
 * @param[in,out]   uart_desc   Instance that contains UART parameters and UART Handler
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
        (void)(uart_desc);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *uart_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   uart_desc   Descriptor that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this UART
 */
static returnCode_t UartSetupIRQs(uartDesc_t *uart_desc, const uartConf_t *uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_conf->default_mode == INTERRUPT_MODE) || (uart_conf->default_mode == DMA_MODE))
    {
        // Set uart inst as the interrupt parameter to pass it to the interrupt routine
        IRQHandlerParam_t param = (IRQHandlerParam_t)uart_desc;
        // Request the interrupt for RX
        return_value = RequestIRQ(uart_conf->irq_no, 5u, UartRxGenericIRQHandler, param);
        if (return_value == RET_SUCCESSFUL)
        {
            // Request the interrupt for TX
            return_value = RequestIRQ(uart_conf->irq_no + 1u, 5u, UartTxGenericIRQHandler, param);
        }
    }

    return return_value;
}

/**
 * @fn              UartCheckRX(uartDesc_t *uart_desc, void *data)
 * @brief           Function that checks the status of a UART reception
 * @param[in,out]   uart_desc   Instance that contains UART parameters and UART Handler
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
        if (uart_desc->handle_struct.rxstate == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_desc->handle_struct.rxstate == HAL_UART_STATE_BUSY_RX)
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
 * @param[in,out]   uart_desc   Instance that contains UART parameters and UART Handler
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
        if (uart_desc->handle_struct.gstate == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_desc->handle_struct.gstate == HAL_UART_STATE_BUSY_TX)
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

/*************************** IRQ Handler Definition **************************/

/**
 * @fn      UartRxGenericIRQHandler(void *param)
 * @brief   Generic UART receiver IRQ Handler
 */
static void UartRxGenericIRQHandler(void *param)
{
    // Get uart inst
    uartDesc_t *uart_desc = (uartDesc_t *)param;

    // Save pre-interrupt status
    HAL_UART_StateTypeDef rx_status = uart_desc->handle_struct.rxstate;

    // Do IRQ
    cmsdk_UartRxIRQHandler(&uart_desc->handle_struct);

    // Check if something has changed
    if ((uart_desc->handle_struct.rxstate != rx_status) && (uart_desc->handle_struct.rxstate == HAL_UART_STATE_READY))
    {
        // RX completed
        if (uart_desc->callback_rx_completed != NULL)
        {
            uart_desc->callback_rx_completed(uart_desc->callback_rx_completed_param);
        }
    }
}

/**
 * @fn      UartTxGenericIRQHandler(void *param)
 * @brief   Generic UART transmitter IRQ Handler
 */
static void UartTxGenericIRQHandler(void *param)
{
    // Get uart inst
    uartDesc_t *uart_desc = (uartDesc_t *)param;

    // Save pre-interrupt status
    HAL_UART_StateTypeDef tx_status = uart_desc->handle_struct.gstate;

    // Do IRQ
    cmsdk_UartTxIRQHandler(&uart_desc->handle_struct);

    // Check if something has changed
    if ((uart_desc->handle_struct.gstate != tx_status) && (uart_desc->handle_struct.gstate == HAL_UART_STATE_READY))
    {
        // TX completed
        if (uart_desc->callback_tx_completed != NULL)
        {
            uart_desc->callback_tx_completed(uart_desc->callback_tx_completed_param);
        }
    }
}