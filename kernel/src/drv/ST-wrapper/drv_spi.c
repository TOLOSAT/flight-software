/**
 * @file    drv_spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for SPI functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_spi.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void SpiGenericIRQHandler(void *param);
static returnCode_t SpiSetupIRQs(spiInst_t *spi_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiInst_t *spi_inst)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if SPI ref is not available for this board or one pointer is null
 */
returnCode_t SpiOpen(spiInst_t *spi_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (spi_inst != NULL)
    {
        // Check and setup spi drive mode
        if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_MASTER_DRIVE))
        {
            spi_inst->handle_struct.Init.Mode = SPI_MODE_MASTER;
        }
        else if ((spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
        {
            spi_inst->handle_struct.Init.Mode = SPI_MODE_SLAVE;
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }

        // Continue if drive mode exists
        if (return_value != RET_INVALID_PARAM)
        {
            spi_inst->handle_struct.Instance = spi_inst->spi_ref;
            spi_inst->handle_struct.Init.BaudRatePrescaler = spi_inst->prescaler;
            spi_inst->handle_struct.Init.Direction = SPI_DIRECTION_2LINES;
            spi_inst->handle_struct.Init.DataSize = SPI_DATASIZE_8BIT;
            spi_inst->handle_struct.Init.CLKPolarity = SPI_POLARITY_LOW;
            spi_inst->handle_struct.Init.CLKPhase = SPI_PHASE_1EDGE;
            spi_inst->handle_struct.Init.NSS = SPI_NSS_SOFT;
            spi_inst->handle_struct.Init.FirstBit = SPI_FIRSTBIT_MSB;
            spi_inst->handle_struct.Init.TIMode = SPI_TIMODE_DISABLE;
            spi_inst->handle_struct.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
            spi_inst->handle_struct.Init.CRCPolynomial = 0x0;
#if defined(STM32H7)
            spi_inst->handle_struct.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
            spi_inst->handle_struct.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
            spi_inst->handle_struct.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
            spi_inst->handle_struct.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
            spi_inst->handle_struct.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
            spi_inst->handle_struct.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
            spi_inst->handle_struct.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
            spi_inst->handle_struct.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
            spi_inst->handle_struct.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
            spi_inst->handle_struct.Init.IOSwap = SPI_IO_SWAP_DISABLE;
#elif defined(STM32F4)
            spi_inst->handle_struct.Init.Mode = SPI_MODE_MASTER;
#else
#error "Current STM32 familly is not supported" 
#endif

            uint32_t test_val = HAL_SPI_Init(&spi_inst->handle_struct);
            if (test_val != HAL_OK)
            {
                return_value = RET_ERROR;
            }
            else
            {
                return_value = SpiSetupIRQs(spi_inst);
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
 * @fn          SpiWrite(spiInst_t *spi_inst, data_t msg, length_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @param[in]   msg         Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if spi is still sending previous message
 * @retval      #RET_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
returnCode_t SpiWrite(spiInst_t *spi_inst, data_t msg, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((spi_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE) || (spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE))
            {
                test_val = HAL_SPI_Transmit(&spi_inst->handle_struct, msg, length, DRV_MAX_DELAY);
            }
            else
            {
                test_val = HAL_SPI_Transmit_IT(&spi_inst->handle_struct, msg, length);
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = RET_NOT_AVAILABLE;
                break;
            default:
                return_value = RET_ERROR;
                break;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SpiRead(spiInst_t *spi_inst, data_t received_msg, data_t transmit_msg, length_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_inst        Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr      Adress of the slave to which the message will be requested
 * @param[out]  received_msg    Message we want to receive
 * @param[in]   transmit_msg    Message we will transmit while we receive (if NULL then 0 will be send instead)
 * @param[in]   length          Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if spi timed out before receiving message
 * @retval      #RET_NOT_AVAILABLE if spi is still receiving previous message
 * @retval      #RET_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
returnCode_t SpiRead(spiInst_t *spi_inst, data_t received_msg, data_t transmit_msg, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((spi_inst != NULL) && (received_msg != NULL) && (length != 0u))
    {
        if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE) || (spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Read with driven mode
            if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE))
            {
                if (transmit_msg == NULL)
                {
                    test_val = HAL_SPI_Receive(&spi_inst->handle_struct, received_msg, length, DRV_MAX_DELAY);
                }
                else
                {
                    test_val = HAL_SPI_TransmitReceive(&spi_inst->handle_struct, transmit_msg, received_msg, length, DRV_MAX_DELAY);
                }
            }
            else
            {
                if (transmit_msg == NULL)
                {
                    test_val = HAL_SPI_Receive_IT(&spi_inst->handle_struct, received_msg, length);
                }
                else
                {
                    test_val = HAL_SPI_TransmitReceive_IT(&spi_inst->handle_struct, transmit_msg, received_msg, length);
                }
            }
            // Check return value
            switch (test_val)
            {
            case HAL_OK:
                return_value = RET_SUCCESSFUL;
                break;
            case HAL_TIMEOUT:
                return_value = RET_TIMEOUT;
                break;
            case HAL_BUSY:
                return_value = RET_NOT_AVAILABLE;
                break;
            default:
                return_value = RET_ERROR;
                break;
            }
        }
        else
        {
            return_value = RET_INVALID_PARAM;
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
 * @retval          #RET_ERROR if io control encountered an error
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
returnCode_t SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (spi_inst != NULL)
    {
        // TO DO : complete IOCTL function
        (void)(spi_inst);
        (void)(cmd);
        (void)(data);
        (void)(data_size);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiClose(spiInst_t *spi_inst)
 * @brief           Function that desinit the SPI connection and puts defaults parameters
 * @param[in,out]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase spi_inst
 */
returnCode_t SpiClose(spiInst_t *spi_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (spi_inst != NULL)
    {
        HAL_SPI_DeInit(&spi_inst->handle_struct);
        return_value = DisableIRQ(spi_inst->irq_no);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SpiSetupIRQs(spiInst_t *spi_inst)
 * @brief       Function that setups interrupt if needed
 * @param[in]   spi_inst    Instance that contains SPI parameters and SPI Handler
 * @retval      #RET_SUCCESSFUL if changing parameters succeed
 * @retval      #RET_INVALID_PARAM if IT is not available for this SPI
 */
static returnCode_t SpiSetupIRQs(spiInst_t *spi_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
    {
        IRQHandlerParam_t param = (IRQHandlerParam_t)&spi_inst->handle_struct;
        return_value = RequestIRQ(spi_inst->irq_no, 5u, SpiGenericIRQHandler, param);
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              SpiGenericIRQHandler(void *param)
 * @brief           Generic SPI Handler
 */
static void SpiGenericIRQHandler(void *param)
{
    spiHandleStruct_t *handle_struct = (spiHandleStruct_t *)param;
    HAL_SPI_IRQHandler(handle_struct);
}