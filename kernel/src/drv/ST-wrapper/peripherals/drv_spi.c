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
static returnCode_t SpiSetupIRQs(spiDesc_t *spi_desc, const spiConf_t *const spi_conf);
static returnCode_t SpiSetUpDMA(spiDesc_t *spi_desc, const spiConf_t *const spi_conf);
static returnCode_t SpiCheckRXTX(spiDesc_t *spi_desc);
static returnCode_t SpiStopRXTX(spiDesc_t *spi_desc);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_desc   Descriptor that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if SPI ref is not available for this board or one pointer is null
 */
returnCode_t SpiOpen(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_desc != NULL) && (spi_conf != NULL))
    {
        spi_desc->handle_struct.Instance               = spi_conf->spi_ref;
        spi_desc->handle_struct.Init.BaudRatePrescaler = spi_conf->prescaler;
        spi_desc->handle_struct.Init.Mode              = SPI_MODE_MASTER;
        spi_desc->handle_struct.Init.Direction         = SPI_DIRECTION_2LINES;
        spi_desc->handle_struct.Init.DataSize          = SPI_DATASIZE_8BIT;
        spi_desc->handle_struct.Init.CLKPolarity       = SPI_POLARITY_LOW;
        spi_desc->handle_struct.Init.CLKPhase          = SPI_PHASE_1EDGE;
        spi_desc->handle_struct.Init.NSS               = SPI_NSS_SOFT;
        spi_desc->handle_struct.Init.FirstBit          = SPI_FIRSTBIT_MSB;
        spi_desc->handle_struct.Init.TIMode            = SPI_TIMODE_DISABLE;
        spi_desc->handle_struct.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
        spi_desc->handle_struct.Init.CRCPolynomial     = 0x0;
#if defined(STM32H7)
        spi_desc->handle_struct.Init.NSSPMode                   = SPI_NSS_PULSE_ENABLE;
        spi_desc->handle_struct.Init.NSSPolarity                = SPI_NSS_POLARITY_LOW;
        spi_desc->handle_struct.Init.FifoThreshold              = SPI_FIFO_THRESHOLD_01DATA;
        spi_desc->handle_struct.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
        spi_desc->handle_struct.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
        spi_desc->handle_struct.Init.MasterSSIdleness           = SPI_MASTER_SS_IDLENESS_00CYCLE;
        spi_desc->handle_struct.Init.MasterInterDataIdleness    = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
        spi_desc->handle_struct.Init.MasterReceiverAutoSusp     = SPI_MASTER_RX_AUTOSUSP_DISABLE;
        spi_desc->handle_struct.Init.MasterKeepIOState          = SPI_MASTER_KEEP_IO_STATE_DISABLE;
        spi_desc->handle_struct.Init.IOSwap                     = SPI_IO_SWAP_DISABLE;
#endif

        HAL_StatusTypeDef test_val = HAL_SPI_Init(&spi_desc->handle_struct);
        if (test_val == HAL_OK)
        {
            // Set current mode
            spi_desc->current_mode = spi_conf->default_mode;
            // Setup DMA if necessary
            if (spi_conf->default_mode == DMA_MODE)
            {
                return_value = SpiSetUpDMA(spi_desc, spi_conf);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Finally setup IRQ
                    return_value = SpiSetupIRQs(spi_desc, spi_conf);
                }
            }
            else
            {
                // Finally setup IRQ
                return_value = SpiSetupIRQs(spi_desc, spi_conf);
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
 * @fn          SpiWrite(spiDesc_t *spi_desc, data_t data, length_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spi is still sending previous message
 */
returnCode_t SpiWrite(spiDesc_t *spi_desc, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_desc != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Write with driven mode
        if (spi_desc->current_mode == DMA_MODE)
        {
            test_val = HAL_SPI_Transmit_DMA(&spi_desc->handle_struct, data, length);
        }
        else if (spi_desc->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_SPI_Transmit_IT(&spi_desc->handle_struct, data, length);
        }
        else if (spi_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_SPI_Transmit(&spi_desc->handle_struct, data, length, DRV_MAX_DELAY);
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
 * @fn          SpiRead(spiDesc_t *spi_desc, data_t data, length_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_desc        Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr      Adress of the slave to which the message will be requested
 * @param[out]  data            Message we want to receive
 * @param[in]   length          Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before receiving message
 * @retval      #RET_NOT_AVAILABLE if spi is still receiving previous message
 */
returnCode_t SpiRead(spiDesc_t *spi_desc, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_desc != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Read with driven mode
        if (spi_desc->current_mode == DMA_MODE)
        {
            if (spi_desc->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive_DMA(&spi_desc->handle_struct, data, length);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive_DMA(&spi_desc->handle_struct, spi_desc->rxtx_data, data, length);
            }
        }
        else if (spi_desc->current_mode == INTERRUPT_MODE)
        {
            if (spi_desc->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive_IT(&spi_desc->handle_struct, data, length);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive_IT(&spi_desc->handle_struct, spi_desc->rxtx_data, data, length);
            }
        }
        else if (spi_desc->current_mode == POLLING_MODE)
        {
            if (spi_desc->rxtx_data_length == 0u)
            {
                test_val = HAL_SPI_Receive(&spi_desc->handle_struct, data, length, DRV_MAX_DELAY);
            }
            else
            {
                test_val = HAL_SPI_TransmitReceive(&spi_desc->handle_struct, spi_desc->rxtx_data, data, length, DRV_MAX_DELAY);
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
 * @fn              SpiIoctl(spiDesc_t *spi_desc, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
returnCode_t SpiIoctl(spiDesc_t *spi_desc, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_desc != NULL)
    {
        switch (cmd)
        {
            case IOCTL_PERIPHERAL_CHECK_RX :
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = SpiCheckRXTX(spi_desc);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
            case IOCTL_PERIPHERAL_STOP_RX :
            case IOCTL_PERIPHERAL_STOP_TX :
                return_value = SpiStopRXTX(spi_desc);
                break;
            case IOCTL_SPI_SET_TX_MSG :
                if (data_size == 0u)
                {
                    spi_desc->rxtx_data        = NULL;
                    spi_desc->rxtx_data_length = 0u;
                }
                else
                {
                    spi_desc->rxtx_data        = data;
                    spi_desc->rxtx_data_length = data_size;
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
 * @fn              SpiClose(spiDesc_t *spi_desc)
 * @brief           Function that desinit the SPI connection and puts defaults parameters
 * @param[in,out]   spi_desc    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase spi_desc
 */
returnCode_t SpiClose(spiDesc_t *spi_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_desc != NULL)
    {
        HAL_SPI_DeInit(&spi_desc->handle_struct);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiSetupIRQs(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   spi_desc   Descriptor that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this SPI
 */
static returnCode_t SpiSetupIRQs(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((spi_conf->default_mode == INTERRUPT_MODE) || (spi_conf->default_mode == DMA_MODE))
    {
        // Set spi inst as the interrupt parameter to pass it to the interrupt routine
        IRQHandlerParam_t param = (IRQHandlerParam_t)spi_desc;
        // Request the interrupt
        return_value = RequestIRQ(spi_conf->irq_no, 5u, SpiGenericIRQHandler, param);
    }

    return return_value;
}

/**
 * @fn              SpiSetUpDMA(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   spi_desc   Descriptor that contains SPI handlers
 * @param[in]       spi_conf   Configuration that contains SPI parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if DMA is not available for this SPI
 */
static returnCode_t SpiSetUpDMA(spiDesc_t *spi_desc, const spiConf_t *const spi_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Check parameter(s)
    if (spi_conf->default_mode == DMA_MODE)
    {
        // First enable clock for DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();

        // Setup DMA RX
        spi_desc->dma_rx_handle_struct.Instance = spi_conf->dma_rx_ref;
#if defined(STM32H7)
        spi_desc->dma_rx_handle_struct.Init.Request = spi_conf->dma_rx_channel;
#elif defined(STM32F4)
        spi_desc->dma_rx_handle_struct.Init.Channel = spi_conf->dma_rx_channel;
#else
#error
#endif
        spi_desc->dma_rx_handle_struct.Init.Direction           = DMA_PERIPH_TO_MEMORY;
        spi_desc->dma_rx_handle_struct.Init.PeriphInc           = DMA_PINC_DISABLE;
        spi_desc->dma_rx_handle_struct.Init.MemInc              = DMA_MINC_ENABLE;
        spi_desc->dma_rx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        spi_desc->dma_rx_handle_struct.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
        spi_desc->dma_rx_handle_struct.Init.Mode                = DMA_NORMAL;
        spi_desc->dma_rx_handle_struct.Init.Priority            = DMA_PRIORITY_LOW;
        spi_desc->dma_rx_handle_struct.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
        spi_desc->dma_rx_handle_struct.Parent                   = &spi_desc->handle_struct;
        spi_desc->handle_struct.hdmarx                          = &spi_desc->dma_rx_handle_struct;

        // Init DMA RX
        test_hal = HAL_DMA_Init(&spi_desc->dma_rx_handle_struct);
        if (test_hal == HAL_OK)
        {
            // Setup DMA TX
            spi_desc->dma_tx_handle_struct.Instance = spi_conf->dma_tx_ref;
#if defined(STM32H7)
            spi_desc->dma_tx_handle_struct.Init.Request = spi_conf->dma_tx_channel;
#elif defined(STM32F4)
            spi_desc->dma_tx_handle_struct.Init.Channel = spi_conf->dma_tx_channel;
#else
#error Architecture is not supported
#endif
            spi_desc->dma_tx_handle_struct.Init.Direction           = DMA_MEMORY_TO_PERIPH;
            spi_desc->dma_tx_handle_struct.Init.PeriphInc           = DMA_PINC_DISABLE;
            spi_desc->dma_tx_handle_struct.Init.MemInc              = DMA_MINC_ENABLE;
            spi_desc->dma_tx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            spi_desc->dma_tx_handle_struct.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
            spi_desc->dma_tx_handle_struct.Init.Mode                = DMA_NORMAL;
            spi_desc->dma_tx_handle_struct.Init.Priority            = DMA_PRIORITY_LOW;
            spi_desc->dma_tx_handle_struct.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
            spi_desc->dma_tx_handle_struct.Parent                   = &spi_desc->handle_struct;
            spi_desc->handle_struct.hdmatx                          = &spi_desc->dma_tx_handle_struct;

            // Init DMA TX
            test_hal = HAL_DMA_Init(&spi_desc->dma_tx_handle_struct);
            if (test_hal == HAL_OK)
            {
                // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)&spi_desc->dma_rx_handle_struct;
                // Request DMA RX interrupt
                return_value = RequestIRQ(spi_conf->dma_rx_irq_no, 8u, SpiGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                    param = (IRQHandlerParam_t)&spi_desc->dma_tx_handle_struct;
                    // Request DMA RX interrupt
                    return_value = RequestIRQ(spi_conf->dma_tx_irq_no, 8u, SpiGenericDMAIRQHandler, param);
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
 * @fn              SpiCheckRXTX(spiDesc_t *spi_desc)
 * @brief           Function that checks the status of a SPI reception and transmission
 * @param[in,out]   spi_desc   Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if SPI is still receiving or transmitting data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiCheckRXTX(spiDesc_t *spi_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_desc != NULL)
    {
        if (spi_desc->handle_struct.State == HAL_SPI_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if ((spi_desc->handle_struct.State == HAL_SPI_STATE_BUSY_RX) || (spi_desc->handle_struct.State == HAL_SPI_STATE_BUSY_TX)
                 || (spi_desc->handle_struct.State == HAL_SPI_STATE_BUSY_TX_RX))
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
 * @fn              SpiStopRXTX(spiDesc_t *spi_desc)
 * @brief           Function that stop the SPI reception and transmission
 * @param[in,out]   spi_desc   Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t SpiStopRXTX(spiDesc_t *spi_desc)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (spi_desc != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((spi_desc->current_mode == DMA_MODE) || (spi_desc->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_SPI_Abort_IT(&spi_desc->handle_struct);
        }
        else if (spi_desc->current_mode == POLLING_MODE)
        {
            test_val = HAL_SPI_Abort(&spi_desc->handle_struct);
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
    spiDesc_t *spi_desc = (spiDesc_t *)param;

    // Save pre-interrupt status
    HAL_SPI_StateTypeDef old_status = spi_desc->handle_struct.State;

    // Do IRQ
    HAL_SPI_IRQHandler(&spi_desc->handle_struct);

    // Check if action has completed
    if ((spi_desc->handle_struct.State == HAL_SPI_STATE_READY))
    {
        if ((old_status == HAL_SPI_STATE_BUSY_RX) || (old_status == HAL_SPI_STATE_BUSY_TX_RX))
        {
            // RX completed
            if (spi_desc->callback_rx_completed != NULL)
            {
                spi_desc->callback_rx_completed(spi_desc->callback_rx_completed_param);
            }
        }
        if ((old_status == HAL_SPI_STATE_BUSY_TX) || (old_status == HAL_SPI_STATE_BUSY_TX_RX))
        {
            // TX completed
            if (spi_desc->callback_tx_completed != NULL)
            {
                spi_desc->callback_tx_completed(spi_desc->callback_tx_completed_param);
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