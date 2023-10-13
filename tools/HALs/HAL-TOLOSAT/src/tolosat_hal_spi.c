/**
 * @file    tolosat_hal_spi.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL SPI functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"
#include <string.h>

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t SpiEnableInterrupt(spiInst_t *spi_inst);
static halStatus_t SpiDisableInterrupt(spiInst_t *spi_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              SpiOpen(spiInst_t *spi_inst)
 * @brief           Function that initialise a SPI connection
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval          #THAL_SUCCESSFUL if creation succeed
 * @retval          #THAL_INVALID_PARAM if SPI ref is not available for this board or one pointer is null
 */
halStatus_t SpiOpen(spiInst_t *spi_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (spi_inst != NULL)
    {
        if (spi_inst->spi_ref == TAPAS_SPI_SDCARD)
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
                return_value = THAL_INVALID_PARAM;
            }

            // Continue if drive mode exists
            if (return_value != THAL_INVALID_PARAM)
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
                TAPAS_SPI_SPECIFIC_INIT(spi_inst);

                uint32_t test_val = HAL_SPI_Init(&spi_inst->handle_struct);
                if (test_val != HAL_OK)
                {
                    return_value = THAL_ERROR;
                }
                else
                {
                    return_value = SpiEnableInterrupt(spi_inst);
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
 * @fn          SpiWrite(spiInst_t *spi_inst, spiMsg_t *msg, spiMsgLength_t length)
 * @brief       Function that write over a SPI connection
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to sent
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if spi timed out before sending message
 * @retval      #THAL_BUSY if spi is still sending previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t SpiWrite(spiInst_t *spi_inst, spiMsg_t *msg, spiMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((spi_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE) || (spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if ((spi_inst->drive_type == SPI_POLLING_MASTER_DRIVE) || (spi_inst->drive_type == SPI_POLLING_SLAVE_DRIVE))
            {
                test_val = HAL_SPI_Transmit(&spi_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            }
            else
            {
                test_val = HAL_SPI_Transmit_IT(&spi_inst->handle_struct, msg, length);
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
 * @fn          SpiRead(spiInst_t *spi_inst, spiMsg_t *received_msg, spiMsg_t *transmit_msg, spiMsgLength_t length)
 * @brief       Function that read over SPI connection
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be requested
 * @param[out]  received_msg Message we want to receive
 * @param[in]   transmit_msg Message we will transmit while we receive (if NULL then 0 will be send instead)
 * @param[in]   length Size of the message we want to receive
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if spi timed out before receiving message
 * @retval      #THAL_BUSY if spi is still receiving previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t SpiRead(spiInst_t *spi_inst, spiMsg_t *received_msg, spiMsg_t *transmit_msg, spiMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

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
                    test_val = HAL_SPI_Receive(&spi_inst->handle_struct, received_msg, length, HAL_MAX_DELAY);
                }
                else
                {
                    test_val = HAL_SPI_TransmitReceive(&spi_inst->handle_struct, transmit_msg, received_msg, length, HAL_MAX_DELAY);
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
 * @fn              SpiIoctl(spiInst_t *spi_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if action cannot be performed because driver is busy
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
halStatus_t SpiIoctl(spiInst_t *spi_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (spi_inst != NULL)
    {
        /* TO DO */
        (void)(spi_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              SpiClose(spiInst_t *spi_inst)
 * @brief           Function that desinit the SPI connection and puts defaults parameters
 * @param[in,out]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval          #THAL_SUCCESSFUL if changing parameters succeed
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase spi_inst
 */
halStatus_t SpiClose(spiInst_t *spi_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    spiInst_t null_inst = {
        .handle_struct = {0},
        .drive_type = 0,
        .spi_ref = 0,
    };

    // Function Core
    if (spi_inst != NULL)
    {
        HAL_SPI_DeInit(&spi_inst->handle_struct);
        return_value = SpiDisableInterrupt(spi_inst);
        *spi_inst = null_inst;
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          SpiEnableInterrupt(spiInst_t *spi_inst)
 * @brief       Function that enables interrupt if needed
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this SPI
 */
static halStatus_t SpiEnableInterrupt(spiInst_t *spi_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
    {
        if (spi_inst->spi_ref == TAPAS_SPI_SDCARD)
        {
            HAL_NVIC_SetPriority(TAPAS_SPI_SDCARD_IRQ_NO, 5, 0);
            HAL_NVIC_EnableIRQ(TAPAS_SPI_SDCARD_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          SpiDisableInterrupt(spiInst_t *spi_inst)
 * @brief       Function that disables interrupt if needed
 * @param[in]   spi_inst Instance that contains SPI parameters and SPI Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this SPI
 */
static halStatus_t SpiDisableInterrupt(spiInst_t *spi_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((spi_inst->drive_type == SPI_IT_MASTER_DRIVE) || (spi_inst->drive_type == SPI_IT_SLAVE_DRIVE))
    {
        if (spi_inst->spi_ref == TAPAS_SPI_SDCARD)
        {
            HAL_NVIC_DisableIRQ(TAPAS_SPI_SDCARD_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}