/**
 * @file    drv_i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for I2C functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_i2c.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void I2cGenericIRQHandler(void *param);
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              I2cOpen(i2cInst_t *i2c_inst)
 * @brief           Function that initialise a I2C connection
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if I2C ref is not available for this board or one pointer is null
 */
returnCode_t I2cOpen(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (i2c_inst != NULL)
    {
        i2c_inst->handle_struct.Instance = i2c_inst->i2c_ref;
        i2c_inst->handle_struct.Init.OwnAddress1 = i2c_inst->own_address;
        i2c_inst->handle_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        i2c_inst->handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2c_inst->handle_struct.Init.OwnAddress2 = 0;
        i2c_inst->handle_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
        i2c_inst->handle_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
#if defined(STM32H7)
        i2c_inst->handle_struct.Init.Timing = 0x307075B1;
#elif defined(STM32F4)
        i2c_inst->handle_struct.Init.ClockSpeed = 100000;
        i2c_inst->handle_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
#else
#error "Current STM32 familly is not supported" 
#endif

        uint32_t test_val = HAL_I2C_Init(&i2c_inst->handle_struct);
        if (test_val != HAL_OK)
        {
            return_value = RET_ERROR;
        }
        else
        {
            return_value = I2cSetupIRQs(i2c_inst);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          I2cWrite(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t data, length_t length)
 * @brief       Function that write over a I2C connection
 * @param[in]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be send
 * @param[in]   data Message we want to send
 * @param[in]   length Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if i2c timed out before sending message
 * @retval      #RET_BUSY if i2c is still sending previous message
 * @retval      #RET_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
returnCode_t I2cWrite(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst != NULL) && (data != NULL) && (slave_addr != 0u) && (length != 0u))
    {
        if ((i2c_inst->drive_type == I2C_POLLING_MASTER_DRIVE) || (i2c_inst->drive_type == I2C_POLLING_SLAVE_DRIVE) || (i2c_inst->drive_type == I2C_IT_MASTER_DRIVE) || (i2c_inst->drive_type == I2C_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Write with driven mode
            if (i2c_inst->drive_type == I2C_POLLING_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Transmit(&i2c_inst->handle_struct, slave_addr, data, length, DRV_MAX_DELAY);
            }
            else if (i2c_inst->drive_type == I2C_POLLING_SLAVE_DRIVE)
            {
                test_val = HAL_I2C_Slave_Transmit(&i2c_inst->handle_struct, data, length, DRV_MAX_DELAY);
            }
            else if (i2c_inst->drive_type == I2C_IT_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Transmit_IT(&i2c_inst->handle_struct, slave_addr, data, length);
            }
            else
            {
                test_val = HAL_I2C_Slave_Transmit_IT(&i2c_inst->handle_struct, data, length);
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
                return_value = RET_BUSY;
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
 * @fn          I2cRead(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t data, length_t length)
 * @brief       Function that read over I2C connection
 * @param[in]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in]   slave_addr Adress of the slave to which the message will be requested
 * @param[out]  data Message we want to receive
 * @param[in]   length Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if i2c timed out before receiving message
 * @retval      #RET_BUSY if i2c is still receiving previous message
 * @retval      #RET_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
returnCode_t I2cRead(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst != NULL) && (data != NULL) && (slave_addr != 0u) && (length != 0u))
    {
        if ((i2c_inst->drive_type == I2C_POLLING_MASTER_DRIVE) || (i2c_inst->drive_type == I2C_POLLING_SLAVE_DRIVE) || (i2c_inst->drive_type == I2C_IT_MASTER_DRIVE) || (i2c_inst->drive_type == I2C_IT_SLAVE_DRIVE))
        {
            uint32_t test_val;
            // Read with driven mode
            if (i2c_inst->drive_type == I2C_POLLING_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Receive(&i2c_inst->handle_struct, slave_addr, data, length, DRV_MAX_DELAY);
            }
            else if (i2c_inst->drive_type == I2C_POLLING_SLAVE_DRIVE)
            {
                test_val = HAL_I2C_Slave_Receive(&i2c_inst->handle_struct, data, length, DRV_MAX_DELAY);
            }
            else if (i2c_inst->drive_type == I2C_IT_MASTER_DRIVE)
            {
                test_val = HAL_I2C_Master_Receive_IT(&i2c_inst->handle_struct, slave_addr, data, length);
            }
            else
            {
                test_val = HAL_I2C_Slave_Receive_IT(&i2c_inst->handle_struct, data, length);
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
                return_value = RET_BUSY;
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
 * @fn              I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @param[in]       cmd IO Control command
 * @param[in,out]   data IO Control command
 * @param[in]       data_size IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_BUSY if action cannot be performed because driver is busy
 * @retval          #RET_ERROR if io control encountered an error
 * @retval          #RET_SUCCESSFUL else
 *
 * @warning This feature is not supported yet so it does nothing
 */
returnCode_t I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (i2c_inst != NULL)
    {
        // TO DO : complete IOCTL function
        (void)(i2c_inst);
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
 * @fn              I2cClose(i2cInst_t *i2c_inst)
 * @brief           Function that desinit the I2C connection and puts defaults parameters
 * @param[in,out]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 *
 * This function erase i2c_inst
 */
returnCode_t I2cClose(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (i2c_inst != NULL)
    {
        HAL_I2C_DeInit(&i2c_inst->handle_struct);
        return_value = DisableIRQ(i2c_inst->irq_no);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          I2cSetupIRQs(i2cInst_t *i2c_inst)
 * @brief       Function that setups interrupt if needed
 * @param[in]   i2c_inst Instance that contains I2C parameters and I2C Handler
 * @retval      #RET_SUCCESSFUL if changing parameters succeed
 * @retval      #RET_INVALID_PARAM if IT is not available for this I2C
 */
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst->drive_type == I2C_IT_MASTER_DRIVE) || (i2c_inst->drive_type == I2C_IT_SLAVE_DRIVE))
    {
        IRQHandlerParam_t param = (IRQHandlerParam_t)&i2c_inst->handle_struct;
        return_value = RequestIRQ(i2c_inst->irq_no, 5u, I2cGenericIRQHandler, param);
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @fn              I2cGenericIRQHandler(void *param)
 * @brief           Generic I2C Handler
 */
static void I2cGenericIRQHandler(void *param)
{
    i2cHandleStruct_t *handle_struct = (i2cHandleStruct_t *)param;
    HAL_I2C_EV_IRQHandler(handle_struct);
}