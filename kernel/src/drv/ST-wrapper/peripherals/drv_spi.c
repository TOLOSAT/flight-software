/**
 * @file    drv_spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SPI functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_spi.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void SpiGenericIRQHandler(void *param);
static void SpiGenericDMAIRQHandler(void *param);
static returnCode_t SpiInitClock(spiInst_t *spi_inst, const spiConf_t *const spi_conf);
static returnCode_t SpiSetupIOs(spiInst_t *spi_inst, const spiConf_t *const spi_conf);
static returnCode_t SpiSetupIRQs(spiInst_t *spi_inst, const spiConf_t *const spi_conf);
static returnCode_t SpiSetUpDMA(spiInst_t *spi_inst, const spiConf_t *const spi_conf);
static returnCode_t SpiCheckRXTX(spiInst_t *spi_inst);
static returnCode_t SpiStopRXTX(spiInst_t *spi_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if SPI ref is not available for this board or one pointer is null
 */
returnCode_t SpiOpen(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_inst != NULL) && (spi_conf != NULL) && (IS_VALID_DRV_MODE(spi_conf->default_mode)))
    {
        // Init peripheral clock
        return_value = SpiInitClock(spi_inst, spi_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = SpiSetupIOs(spi_inst, spi_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup SPI
                spi_inst->handle_struct.Instance               = spi_conf->periph;
                spi_inst->handle_struct.Init.BaudRatePrescaler = spi_conf->prescaler;
                spi_inst->handle_struct.Init.Mode              = SPI_MODE_MASTER;
                spi_inst->handle_struct.Init.Direction         = SPI_DIRECTION_2LINES;
                spi_inst->handle_struct.Init.DataSize          = SPI_DATASIZE_8BIT;
                spi_inst->handle_struct.Init.CLKPolarity       = SPI_POLARITY_LOW;
                spi_inst->handle_struct.Init.CLKPhase          = SPI_PHASE_1EDGE;
                spi_inst->handle_struct.Init.NSS               = SPI_NSS_SOFT;
                spi_inst->handle_struct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
                spi_inst->handle_struct.Init.TIMode            = SPI_TIMODE_DISABLE;
                spi_inst->handle_struct.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
                spi_inst->handle_struct.Init.CRCPolynomial     = 0x0;
#if defined(STM32H7)
                spi_inst->handle_struct.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
                spi_inst->handle_struct.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
                spi_inst->handle_struct.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
                spi_inst->handle_struct.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
                spi_inst->handle_struct.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
                spi_inst->handle_struct.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
                spi_inst->handle_struct.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
                spi_inst->handle_struct.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
                spi_inst->handle_struct.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
                spi_inst->handle_struct.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
#endif

                HAL_StatusTypeDef test_val = HAL_SPI_Init(&spi_inst->handle_struct);
                if (test_val == HAL_OK)
                {
                    // Link the conf pointer
                    spi_inst->p_conf = spi_conf;
                    // Set current mode
                    spi_inst->current_mode = spi_conf->default_mode;
                    // Setup DMA if necessary
                    if (spi_conf->default_mode == DMA_MODE)
                    {
                        return_value = SpiSetUpDMA(spi_inst, spi_conf);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Finally setup IRQ
                            return_value = SpiSetupIRQs(spi_inst, spi_conf);
                        }
                    }
                    else
                    {
                        // Finally setup IRQ
                        return_value = SpiSetupIRQs(spi_inst, spi_conf);
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
 * @fn          SpiWrite(spiInst_t *spi_inst, data_t data, length_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spi is still sending previous message
 */
returnCode_t SpiWrite(spiInst_t *spi_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Write with driven mode
        if (spi_inst->current_mode == DMA_MODE)
        {
            test_val = HAL_SPI_Transmit_DMA(&spi_inst->handle_struct, data, length);
        }
        else if (spi_inst->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_SPI_Transmit_IT(&spi_inst->handle_struct, data, length);
        }
        else if (spi_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_SPI_Transmit(&spi_inst->handle_struct, data, length, DRV_MAX_DELAY);
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
 * @fn          SpiRead(spiInst_t *spi_inst, data_t data, length_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_inst        Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr      Adress of the slave to which the message will be requested
 * @param[out]  data            Message we want to receive
 * @param[in]   length          Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before receiving message
 * @retval      #RET_NOT_AVAILABLE if spi is still receiving previous message
 */
returnCode_t SpiRead(spiInst_t *spi_inst, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Read with driven mode
        if (spi_inst->current_mode == DMA_MODE)
        {
            if (spi_inst->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive_DMA(&spi_inst->handle_struct, data, length);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive_DMA(&spi_inst->handle_struct, spi_inst->rxtx_data, data, length);
            }
        }
        else if (spi_inst->current_mode == INTERRUPT_MODE)
        {
            if (spi_inst->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive_IT(&spi_inst->handle_struct, data, length);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive_IT(&spi_inst->handle_struct, spi_inst->rxtx_data, data, length);
            }
        }
        else if (spi_inst->current_mode == POLLING_MODE)
        {
            if (spi_inst->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive(&spi_inst->handle_struct, data, length, DRV_MAX_DELAY);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive(&spi_inst->handle_struct, spi_inst->rxtx_data, data, length, DRV_MAX_DELAY);
            }
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
 * @fn              SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
returnCode_t SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_PERIPHERAL_DRV_MODE :
                if ((data_size == sizeof(drivingMode_t)) && (IS_VALID_DRV_MODE(*(drivingMode_t *)data))
                    && (*(drivingMode_t *)data <= spi_inst->p_conf->default_mode))
                {
                    spi_inst->current_mode = *(drivingMode_t *)data;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_PERIPHERAL_CHECK_RX :
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = SpiCheckRXTX(spi_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
            case IOCTL_PERIPHERAL_STOP_RX :
            case IOCTL_PERIPHERAL_STOP_TX :
                return_value = SpiStopRXTX(spi_inst);
                break;
            case IOCTL_SPI_SET_TX_MSG :
                if (data_size == 0u)
                {
                    spi_inst->rxtx_data        = NULL;
                    spi_inst->rxtx_data_length = 0u;
                }
                else
                {
                    spi_inst->rxtx_data        = data;
                    spi_inst->rxtx_data_length = data_size;
                }
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
 * @fn              SpiClose(spiInst_t *spi_inst)
 * @brief           Function that desinit the SPI connection
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t SpiClose(spiInst_t *spi_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_inst != NULL)
    {
        HAL_SPI_DeInit(&spi_inst->handle_struct);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiInitClock(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that setups SPI peripheral clock
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t SpiInitClock(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(spi_inst);

    // Check parameter(s)
    if ((spi_inst != NULL) && (spi_conf != NULL))
    {
        // Select the peripheral clock
        switch ((uintptr_t)spi_conf->periph)
        {
            case SPI1_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI1;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI1_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI1_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#if defined(SPI2)
            case SPI2_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI2;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI2_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI2_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI2 */
#if defined(SPI3)
            case SPI3_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI3;
                spi_peripheral_clock_settings.Spi123ClockSelection     = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI3_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI3_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI3 */
#if defined(SPI4)
            case SPI4_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI4;
                spi_peripheral_clock_settings.Spi45ClockSelection      = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI4_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI4_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI4 */
#if defined(SPI5)
            case SPI5_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI5;
                spi_peripheral_clock_settings.Spi45ClockSelection      = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI5_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI5_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI5 */
#if defined(SPI6)
            case SPI6_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef spi_peripheral_clock_settings = { 0 };
                spi_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_SPI6;
                spi_peripheral_clock_settings.Spi6ClockSelection       = spi_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&spi_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_SPI6_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_SPI6_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* SPI6 */
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
 * @fn              SpiSetupIOs(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this SPI
 */
static returnCode_t SpiSetupIOs(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(spi_inst);

    // Check parameter(s)
    if ((spi_inst != NULL) && (spi_conf != NULL))
    {
        // First init SCK IO
        return_value = SetupIO(&spi_conf->io_sck);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init MISO IO
            return_value = SetupIO(&spi_conf->io_miso);
            if (return_value == RET_SUCCESSFUL)
            {
                // Then init MOSI IO
                return_value = SetupIO(&spi_conf->io_mosi);
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSetupIRQs(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this SPI
 */
static returnCode_t SpiSetupIRQs(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_inst != NULL) && (spi_conf != NULL))
    {
        if ((spi_conf->default_mode == INTERRUPT_MODE) || (spi_conf->default_mode == DMA_MODE))
        {
            // Set spi inst as the interrupt parameter to pass it to the interrupt routine
            IRQHandlerParam_t param = (IRQHandlerParam_t)spi_inst;
            // Request the interrupt
            return_value = RequestIRQ(spi_conf->irq_no, 5u, SpiGenericIRQHandler, param);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSetUpDMA(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   spi_inst   Instance that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if DMA is not available for this SPI
 */
static returnCode_t SpiSetUpDMA(spiInst_t *spi_inst, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_inst != NULL) && (spi_conf != NULL) && (spi_conf->default_mode == DMA_MODE))
    {
        // First Setup RX DMA
        return_value = SetUpDMA(&spi_inst->dma_rx_handle_struct, &spi_conf->dma_rx);
        // Setup parents / children
        spi_inst->dma_rx_handle_struct.Parent = &spi_inst->handle_struct;
        spi_inst->handle_struct.hdmarx        = &spi_inst->dma_rx_handle_struct;
        if (return_value == RET_SUCCESSFUL)
        {
            // First Setup RX DMA
            return_value = SetUpDMA(&spi_inst->dma_tx_handle_struct, &spi_conf->dma_tx);
            // Setup parents / children
            spi_inst->dma_tx_handle_struct.Parent = &spi_inst->handle_struct;
            spi_inst->handle_struct.hdmatx        = &spi_inst->dma_tx_handle_struct;
            if (return_value == RET_SUCCESSFUL)
            {
                // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)&spi_inst->dma_rx_handle_struct;
                // Request DMA RX interrupt
                return_value = RequestIRQ(spi_conf->dma_rx.irq_no, 8u, SpiGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                    param = (IRQHandlerParam_t)&spi_inst->dma_tx_handle_struct;
                    // Request DMA TX interrupt
                    return_value = RequestIRQ(spi_conf->dma_tx.irq_no, 8u, SpiGenericDMAIRQHandler, param);
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
 * @fn              SpiCheckRXTX(spiInst_t *spi_inst)
 * @brief           Function that checks the status of a SPI reception and transmission
 * @param[in,out]   spi_inst   Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if SPI is still receiving or transmitting data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiCheckRXTX(spiInst_t *spi_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_inst != NULL)
    {
        if (spi_inst->handle_struct.State == HAL_SPI_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if ((spi_inst->handle_struct.State == HAL_SPI_STATE_BUSY_RX) || (spi_inst->handle_struct.State == HAL_SPI_STATE_BUSY_TX)
                 || (spi_inst->handle_struct.State == HAL_SPI_STATE_BUSY_TX_RX))
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
 * @fn              SpiStopRXTX(spiInst_t *spi_inst)
 * @brief           Function that stop the SPI reception and transmission
 * @param[in,out]   spi_inst   Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiStopRXTX(spiInst_t *spi_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_inst != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((spi_inst->current_mode == DMA_MODE) || (spi_inst->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_SPI_Abort_IT(&spi_inst->handle_struct);
        }
        else if (spi_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_SPI_Abort(&spi_inst->handle_struct);
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
 * @fn              SpiGenericIRQHandler(void *param)
 * @brief           Generic SPI IRQ Handler
 */
static void SpiGenericIRQHandler(void *param)
{
    // Get spi inst
    spiInst_t *spi_inst = (spiInst_t *)param;

    // Save pre-interrupt status
    HAL_SPI_StateTypeDef old_status = spi_inst->handle_struct.State;

    // Do IRQ
    HAL_SPI_IRQHandler(&spi_inst->handle_struct);

    // Check if action has completed
    if ((spi_inst->handle_struct.State == HAL_SPI_STATE_READY))
    {
        if ((old_status == HAL_SPI_STATE_BUSY_RX) || (old_status == HAL_SPI_STATE_BUSY_TX_RX))
        {
            // RX completed
            if (spi_inst->callback_rx_completed != NULL)
            {
                spi_inst->callback_rx_completed(spi_inst->callback_rx_completed_param);
            }
        }
        if ((old_status == HAL_SPI_STATE_BUSY_TX) || (old_status == HAL_SPI_STATE_BUSY_TX_RX))
        {
            // TX completed
            if (spi_inst->callback_tx_completed != NULL)
            {
                spi_inst->callback_tx_completed(spi_inst->callback_tx_completed_param);
            }
        }
    }
}

/**
 * @fn      SpiGenericDMAIRQHandler(void *param)
 * @brief   Generic SPI DMA IRQ Handler
 */
static void SpiGenericDMAIRQHandler(void *param)
{
    DMAHandleStruct_t *handle_struct = (DMAHandleStruct_t *)param;
    HAL_DMA_IRQHandler(handle_struct);
}