/**
 * @file    tolosat_hal_iic.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL IIC functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static halStatus_t IicEnableInterrupt(iicInst_t *iic_inst);
static halStatus_t IicDisableInterrupt(iicInst_t *iic_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              IicOpen(iicInst_t *iic_inst)
 * @brief           Function that initialise a IIC connection
 * @param[in,out]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval          #THAL_SUCCESSFUL if creation succeed
 * @retval          #THAL_INVALID_PARAM if I2C ref is not available for this board or one pointer is null
 */
halStatus_t IicOpen(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (iic_inst != NULL)
    {
        if (iic_inst->iic_ref == TAPAS_I2C_AVIONIC)
        {
            iic_inst->handle_struct.Instance = iic_inst->iic_ref;
            iic_inst->handle_struct.Init.OwnAddress1 = iic_inst->own_address;
            iic_inst->handle_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
            iic_inst->handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
            iic_inst->handle_struct.Init.OwnAddress2 = 0;
            iic_inst->handle_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
            iic_inst->handle_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
            TAPAS_IIC_SPECIFIC_INIT(iic_inst);

            uint32_t test_val = HAL_I2C_Init(&iic_inst->handle_struct);
            if (test_val != HAL_OK)
            {
                return_value = THAL_ERROR;
            }
            else
            {
                return_value = IicEnableInterrupt(iic_inst);
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
 * @fn          IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
 * @brief       Function that write over a IIC connection
 * @param[in]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be send
 * @param[in]   msg Message we want to send
 * @param[in]   length Size of the message we want to sent
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if iic timed out before sending message
 * @retval      #THAL_BUSY if iic is still sending previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((iic_inst != NULL) && (msg != NULL) && (slave_addr != 0u) && (length != 0u))
    {
        if ((iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE) || (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE) || (iic_inst->drive_type == IIC_IT_MASTER_DRIVE) || (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if (iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Transmit(&iic_inst->handle_struct, slave_addr << 1, msg, length, HAL_MAX_DELAY);
            }
            else if (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE)
            {
                test_val = HAL_I2C_Slave_Transmit(&iic_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            }
            else if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Transmit_IT(&iic_inst->handle_struct, slave_addr << 1, msg, length);
            }
            else
            {
                test_val = HAL_I2C_Slave_Transmit_IT(&iic_inst->handle_struct, msg, length);
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
 * @fn          IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
 * @brief       Function that read over IIC connection
 * @param[in]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be requested
 * @param[out]  msg Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #THAL_SUCCESSFUL if message sent successfully
 * @retval      #THAL_INVALID_PARAM if one pointer is null
 * @retval      #THAL_TIMEOUT if iic timed out before receiving message
 * @retval      #THAL_BUSY if iic is still receiving previous message
 * @retval      #THAL_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((iic_inst != NULL) && (msg != NULL) && (slave_addr != 0u) && (length != 0u))
    {
        if ((iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE) || (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE) || (iic_inst->drive_type == IIC_IT_MASTER_DRIVE) || (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Read with driven mode
            if (iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Receive(&iic_inst->handle_struct, slave_addr, msg, length, HAL_MAX_DELAY);
            }
            else if (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE)
            {
                test_val = HAL_I2C_Slave_Receive(&iic_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            }
            else if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Receive_IT(&iic_inst->handle_struct, slave_addr, msg, length);
            }
            else
            {
                test_val = HAL_I2C_Slave_Receive_IT(&iic_inst->handle_struct, msg, length);
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
 * @fn              IicIoctl(iicInst_t *iic_inst, halIoCtlCmd_t io_cmd)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 * @retval          #THAL_BUSY if action cannot be performed because driver is busy
 * @retval          #THAL_ERROR if io control encountered an error
 * @retval          #THAL_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
halStatus_t IicIoctl(iicInst_t *iic_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (iic_inst != NULL)
    {
        /* TO DO */
        (void)(iic_inst);
        (void)(io_cmd);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              IicClose(iicInst_t *iic_inst)
 * @brief           Function that desinit the IIC connection and puts defaults parameters
 * @param[in,out]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval          #THAL_SUCCESSFUL if changing parameters succeed
 * @retval          #THAL_INVALID_PARAM if instance is a null pointer
 *
 * This function erase iic_inst
 */
halStatus_t IicClose(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;
    iicInst_t null_inst = {
        .handle_struct = {0},
        .drive_type = 0,
        .iic_ref = 0,
    };

    // Function Core
    if (iic_inst != NULL)
    {
        HAL_I2C_DeInit(&iic_inst->handle_struct);
        return_value = IicDisableInterrupt(iic_inst);
        *iic_inst = null_inst;
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          IicEnableInterrupt(iicInst_t *iic_inst)
 * @brief       Function that enables interrupt if needed
 * @param[in]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this IIC
 */
static halStatus_t IicEnableInterrupt(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((iic_inst->drive_type == IIC_IT_MASTER_DRIVE) || (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE))
    {
        if (iic_inst->iic_ref == TAPAS_I2C_AVIONIC)
        {
            HAL_NVIC_SetPriority(TAPAS_I2C_AVIONIC_EVT_IRQ_NO, 5, 0);
            HAL_NVIC_EnableIRQ(TAPAS_I2C_AVIONIC_EVT_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}

/**
 * @fn          IicDisableInterrupt(iicInst_t *iic_inst)
 * @brief       Function that disables interrupt if needed
 * @param[in]   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval      #THAL_SUCCESSFUL if changing parameters succeed
 * @retval      #THAL_INVALID_PARAM if IT is not available for this IIC
 */
static halStatus_t IicDisableInterrupt(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((iic_inst->drive_type == IIC_IT_MASTER_DRIVE) || (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE))
    {
        if (iic_inst->iic_ref == TAPAS_I2C_AVIONIC)
        {
            HAL_NVIC_DisableIRQ(TAPAS_I2C_AVIONIC_EVT_IRQ_NO);
        }
        else
        {
            return_value = THAL_INVALID_PARAM;
        }
    }

    return return_value;
}