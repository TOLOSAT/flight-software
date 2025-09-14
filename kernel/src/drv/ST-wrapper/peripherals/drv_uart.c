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
static returnCode_t UartInitClock(uartInst_t *uart_inst, const uartConf_t *const uart_conf);
static returnCode_t UartDeInitClock(uartInst_t *uart_inst);
static returnCode_t UartSetupIOs(uartInst_t *uart_inst, const uartConf_t *const uart_conf);
static returnCode_t UartSetupIRQs(uartInst_t *uart_inst, const uartConf_t *const uart_conf);
static returnCode_t UartSetUpDMA(uartInst_t *uart_inst, const uartConf_t *const uart_conf);
static returnCode_t UartCheckRX(uartInst_t *uart_inst);
static returnCode_t UartCheckTX(uartInst_t *uart_inst);
static returnCode_t UartStopRX(uartInst_t *uart_inst);
static returnCode_t UartStopTX(uartInst_t *uart_inst);
static returnCode_t UartStopRXTX(uartInst_t *uart_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              UartOpen(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that initialise a UART connection
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if UART ref is not available for this board, baudrate or one pointer is null
 */
returnCode_t UartOpen(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_inst != NULL) && (uart_conf != NULL) && (IS_VALID_DRV_MODE(uart_conf->default_mode)) && (uart_conf->baudrate != 0u))
    {
        // Init peripheral clock
        return_value = UartInitClock(uart_inst, uart_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = UartSetupIOs(uart_inst, uart_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup UART
                uart_inst->handle_struct.Instance          = uart_conf->periph;
                uart_inst->handle_struct.Init.BaudRate     = uart_conf->baudrate;
                uart_inst->handle_struct.Init.WordLength   = UART_WORDLENGTH_8B;
                uart_inst->handle_struct.Init.StopBits     = UART_STOPBITS_1;
                uart_inst->handle_struct.Init.Parity       = UART_PARITY_NONE;
                uart_inst->handle_struct.Init.Mode         = UART_MODE_TX_RX;
                uart_inst->handle_struct.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
                uart_inst->handle_struct.Init.OverSampling = UART_OVERSAMPLING_16;

                // Init UART
                HAL_StatusTypeDef test_val = HAL_UART_Init(&uart_inst->handle_struct);
                if (test_val == HAL_OK)
                {
                    // Link the conf pointer
                    uart_inst->p_conf = uart_conf;
                    // Set current mode
                    uart_inst->current_mode = uart_conf->default_mode;
                    // Setup DMA if necessary
                    if (uart_conf->default_mode == DMA_MODE)
                    {
                        return_value = UartSetUpDMA(uart_inst, uart_conf);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Finally setup IRQ
                            return_value = UartSetupIRQs(uart_inst, uart_conf);
                        }
                    }
                    else
                    {
                        // Finally setup IRQ
                        return_value = UartSetupIRQs(uart_inst, uart_conf);
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
 * @fn          UartWrite(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that write over a UART connection
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to send
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
returnCode_t UartWrite(uartInst_t *uart_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Write with driven mode
        if (uart_inst->current_mode == DMA_MODE)
        {
            test_val = HAL_UART_Transmit_DMA(&uart_inst->handle_struct, data, length);
        }
        else if (uart_inst->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_UART_Transmit_IT(&uart_inst->handle_struct, data, length);
        }
        else if (uart_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_Transmit(&uart_inst->handle_struct, data, length, DRV_MAX_DELAY);
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
 * @fn          UartRead(uartInst_t *uart_inst, data_t data, length_t length)
 * @brief       Function that read over UART connection
 * @param[in]   uart_inst   Instance that contains UART parameters and UART Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if uart timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if uart is still sending previous message
 */
returnCode_t UartRead(uartInst_t *uart_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if (uart_inst->current_mode == DMA_MODE)
        {
            test_val = HAL_UARTEx_ReceiveToIdle_DMA(&uart_inst->handle_struct, data, length);
        }
        else if (uart_inst->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_UARTEx_ReceiveToIdle_IT(&uart_inst->handle_struct, data, length);
        }
        else if (uart_inst->current_mode == POLLING_MODE)
        {
            uint16_t nb_byte_received = 0;

            test_val = HAL_UARTEx_ReceiveToIdle(&uart_inst->handle_struct, data, length, &nb_byte_received, DRV_MAX_DELAY);
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
 * @fn              UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t UartIoctl(uartInst_t *uart_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused parameters
    (void)(data);
    (void)(data_size);

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_PERIPHERAL_DRV_MODE :
                if ((data_size == sizeof(drivingMode_t)) && (IS_VALID_DRV_MODE(*(drivingMode_t *)data))
                    && (*(drivingMode_t *)data <= uart_inst->p_conf->default_mode))
                {
                    uart_inst->current_mode = *(drivingMode_t *)data;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_PERIPHERAL_CHECK_RX :
                return_value = UartCheckRX(uart_inst);
                break;
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = UartCheckTX(uart_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_RX :
                return_value = UartStopRX(uart_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_TX :
                return_value = UartStopTX(uart_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
                return_value = UartStopRXTX(uart_inst);
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
 * @fn              UartClose(uartInst_t *uart_inst)
 * @brief           Function that desinit the UART connection
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t UartClose(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        HAL_UART_DeInit(&uart_inst->handle_struct);
        (void)UartDeInitClock(uart_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartInitClock(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that setups UART peripheral clock
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t UartInitClock(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(uart_inst);

    // Check parameter(s)
    if ((uart_inst != NULL) && (uart_conf != NULL))
    {
        // Select the peripheral clock
        switch ((uintptr_t)uart_conf->periph)
        {
            case USART1_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_USART1;
                uart_peripheral_clock_settings.Usart16ClockSelection    = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_USART1_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_USART1_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#if defined(USART2)
            case USART2_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_USART2;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_USART2_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_USART2_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* USART2 */
#if defined(USART3)
            case USART3_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_USART3;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_USART3_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_USART3_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* USART3 */
#if defined(UART4)
            case UART4_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_UART4;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_UART4_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_UART4_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* UART4 */
#if defined(UART5)
            case UART5_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_UART5;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_UART5_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_UART5_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* UART5 */
#if defined(USART6)
            case USART6_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_USART6;
                uart_peripheral_clock_settings.Usart16ClockSelection    = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_USART6_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_USART6_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* USART6 */
#if defined(UART7)
            case UART7_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_UART7;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_UART7_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_UART7_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* UART7 */
#if defined(UART8)
            case UART8_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef uart_peripheral_clock_settings  = { 0 };
                uart_peripheral_clock_settings.PeriphClockSelection      = RCC_PERIPHCLK_UART8;
                uart_peripheral_clock_settings.Usart234578ClockSelection = uart_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&uart_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_UART8_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_UART8_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* UART8 */
            default :
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
 * @fn              UartDeInitClock(uartInst_t *uart_inst)
 * @brief           Function that disables UART peripheral clock
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t UartDeInitClock(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        // Select the peripheral clock
        switch ((uintptr_t)uart_inst->p_conf->periph)
        {
            case USART1_BASE :
            {
                __HAL_RCC_USART1_CLK_DISABLE();
                break;
            }
#if defined(USART2)
            case USART2_BASE :
            {
                __HAL_RCC_USART2_CLK_DISABLE();
                break;
            }
#endif /* USART2 */
#if defined(USART3)
            case USART3_BASE :
            {
                __HAL_RCC_USART3_CLK_DISABLE();
                break;
            }
#endif /* USART3 */
#if defined(UART4)
            case UART4_BASE :
            {
                __HAL_RCC_UART4_CLK_DISABLE();
                break;
            }
#endif /* UART4 */
#if defined(UART5)
            case UART5_BASE :
            {
                __HAL_RCC_UART5_CLK_DISABLE();
                break;
            }
#endif /* UART5 */
#if defined(USART6)
            case USART6_BASE :
            {
                __HAL_RCC_USART6_CLK_DISABLE();
                break;
            }
#endif /* USART6 */
#if defined(UART7)
            case UART7_BASE :
            {
                __HAL_RCC_UART7_CLK_DISABLE();
                break;
            }
#endif /* UART7 */
#if defined(UART8)
            case UART8_BASE :
            {
                __HAL_RCC_UART8_CLK_DISABLE();
                break;
            }
#endif /* UART8 */
            default :
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
 * @fn              UartSetupIOs(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this UART
 */
static returnCode_t UartSetupIOs(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(uart_inst);

    // Check parameter(s)
    if ((uart_inst != NULL) && (uart_conf != NULL))
    {
        // First init RX IO
        return_value = SetupIO(&uart_conf->io_rx);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init TX IO
            return_value = SetupIO(&uart_conf->io_tx);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartSetupIRQs(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this UART
 */
static returnCode_t UartSetupIRQs(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_inst != NULL) && (uart_conf != NULL))
    {
        if ((uart_conf->default_mode == INTERRUPT_MODE) || (uart_conf->default_mode == DMA_MODE))
        {
            // Set uart inst as the interrupt parameter to pass it to the interrupt routine
            IRQHandlerParam_t param = (IRQHandlerParam_t)uart_inst;
            // Request the interrupt
            return_value = RequestIRQ(uart_conf->irq_no, uart_conf->irq_prio, UartGenericIRQHandler, param);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartSetUpDMA(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @param[in]       uart_conf   Configuration that contains UART parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if DMA is not available for this UART
 */
static returnCode_t UartSetUpDMA(uartInst_t *uart_inst, const uartConf_t *const uart_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((uart_inst != NULL) && (uart_conf != NULL) && (uart_conf->default_mode == DMA_MODE))
    {
        // First Setup RX DMA
        return_value = SetUpDMA(&uart_inst->dma_rx_handle_struct, &uart_conf->dma_rx);
        // Setup parents / children
        uart_inst->dma_rx_handle_struct.Parent = &uart_inst->handle_struct;
        uart_inst->handle_struct.hdmarx        = &uart_inst->dma_rx_handle_struct;
        if (return_value == RET_SUCCESSFUL)
        {
            // First Setup RX DMA
            return_value = SetUpDMA(&uart_inst->dma_tx_handle_struct, &uart_conf->dma_tx);
            // Setup parents / children
            uart_inst->dma_tx_handle_struct.Parent = &uart_inst->handle_struct;
            uart_inst->handle_struct.hdmatx        = &uart_inst->dma_tx_handle_struct;
            if (return_value == RET_SUCCESSFUL)
            {
                // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)&uart_inst->dma_rx_handle_struct;
                // Request DMA RX interrupt
                return_value = RequestIRQ(uart_conf->dma_rx.irq_no, uart_conf->dma_rx.irq_prio, UartGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                    param = (IRQHandlerParam_t)&uart_inst->dma_tx_handle_struct;
                    // Request DMA TX interrupt
                    return_value = RequestIRQ(uart_conf->dma_tx.irq_no, uart_conf->dma_tx.irq_prio, UartGenericDMAIRQHandler, param);
                }
            }
            else
            {
                return_value = RET_ERROR;
            }
        }
        else
        {
            return_value = RET_ERROR;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              UartCheckRX(uartInst_t *uart_inst)
 * @brief           Function that checks the status of a UART reception
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if UART is still receiving data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartCheckRX(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.RxState == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_inst->handle_struct.RxState == HAL_UART_STATE_BUSY_RX)
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
 * @fn              UartCheckTX(uartInst_t *uart_inst)
 * @brief           Function that checks the status of a UART trransmission
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if UART is still transfering data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartCheckTX(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        if (uart_inst->handle_struct.gState == HAL_UART_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if (uart_inst->handle_struct.gState == HAL_UART_STATE_BUSY_TX)
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
 * @fn              UartStopRX(uartInst_t *uart_inst)
 * @brief           Function that stop the UART reception
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopRX(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_inst->current_mode == DMA_MODE) || (uart_inst->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_AbortReceive_IT(&uart_inst->handle_struct);
        }
        else if (uart_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_AbortReceive(&uart_inst->handle_struct);
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
 * @fn              UartStopTX(uartInst_t *uart_inst)
 * @brief           Function that stop the UART transmission
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopTX(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_inst->current_mode == DMA_MODE) || (uart_inst->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_AbortTransmit_IT(&uart_inst->handle_struct);
        }
        else if (uart_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_AbortTransmit(&uart_inst->handle_struct);
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
 * @fn              UartStopRXTX(uartInst_t *uart_inst)
 * @brief           Function that stop the UART reception and transmission
 * @param[in,out]   uart_inst   Instance that contains UART handlers
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t UartStopRXTX(uartInst_t *uart_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (uart_inst != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((uart_inst->current_mode == DMA_MODE) || (uart_inst->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_UART_Abort_IT(&uart_inst->handle_struct);
        }
        else if (uart_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_UART_Abort(&uart_inst->handle_struct);
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
    uartInst_t *uart_inst = (uartInst_t *)param;

    // Save pre-interrupt status
    HAL_UART_StateTypeDef tx_status = uart_inst->handle_struct.gState;
    HAL_UART_StateTypeDef rx_status = uart_inst->handle_struct.RxState;

    // Do IRQ
    HAL_UART_IRQHandler(&uart_inst->handle_struct);

    // Check if something has changed
    if ((uart_inst->handle_struct.RxState != rx_status) && (uart_inst->handle_struct.RxState == HAL_UART_STATE_READY))
    {
        // RX completed
        if (uart_inst->callback_rx_completed != NULL)
        {
            uart_inst->callback_rx_completed(uart_inst->callback_rx_completed_param);
        }
    }
    if ((uart_inst->handle_struct.gState != tx_status) && (uart_inst->handle_struct.gState == HAL_UART_STATE_READY))
    {
        // TX completed
        if (uart_inst->callback_tx_completed != NULL)
        {
            uart_inst->callback_tx_completed(uart_inst->callback_tx_completed_param);
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