/**
 * @file tolosat_hal_iic.c
 * @author Merlin Kooshmanian
 * @brief Source file for TOLOSAT HAL IIC functions
 * @date 30/04/2023
 *
 * Last Update : 30/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

static halStatus_t IicEnableInterrupt(iicInst_t *iic_inst);
static halStatus_t IicDisableInterrupt(iicInst_t *iic_inst);

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @fn      IicOpen(iicInst_t *iic_inst)
 * @brief   Function that initialise a IIC connection
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if creation succeed
 * @retval  FCT_INVALID_PARAM if I2C ref is not available for this board, baudrate or one pointer is null
 */
halStatus_t IicOpen(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val;

    // Function Core
    if (iic_inst != NULL)
    {
#if defined(STM32F411xE)
        if (iic_inst->iic_ref == I2C1 || iic_inst->iic_ref == I2C2 || iic_inst->iic_ref == I2C3)
#elif defined(STM32F103xB)
            if (iic_inst->iic_ref == I2C1 || iic_inst->iic_ref == I2C2)
#elif defined(STM32H745xx)
            if (iic_inst->iic_ref == I2C1 || iic_inst->iic_ref == I2C2 || iic_inst->iic_ref == I2C3 || iic_inst->iic_ref == I2C4)
#else
#error "Board is not supported"
#endif
            {
                iic_inst->handle_struct.Instance = iic_inst->iic_ref;
#if defined(STM32F411xE) || defined(STM32F103xB)
                iic_inst->handle_struct.Init.ClockSpeed = 100000;
                iic_inst->handle_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
#elif defined(STM32H745xx)
                iic_inst->handle_struct.Init.Timing = 0x307075B1;
#else
#error "Board is not supported"
#endif
                iic_inst->handle_struct.Init.OwnAddress1 = iic_inst->own_address;
                iic_inst->handle_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
                iic_inst->handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
                iic_inst->handle_struct.Init.OwnAddress2 = 0;
                iic_inst->handle_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
                iic_inst->handle_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

                test_val = HAL_I2C_Init(&iic_inst->handle_struct);
                if (test_val != HAL_OK)
                {
                    return_value = FCT_ERROR;
                }
                else
                {
                    return_value = IicEnableInterrupt(iic_inst);
                }
            }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn      IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
 * @brief   Function that write over a IIC connection
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @param   slave_addr Adress of the slave to which the message will be send
 * @param   msg Message we want to send
 * @param   length Size of the message we want to sent
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {
        uint32_t test_val;
        if (iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE)
        {
            test_val = HAL_I2C_Master_Transmit(&iic_inst->handle_struct, slave_addr << 1, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE)
        {
            test_val = HAL_I2C_Slave_Transmit(&iic_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE)
        {
            test_val = HAL_I2C_Master_Transmit_IT(&iic_inst->handle_struct, slave_addr << 1, msg, length);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE)
        {
            test_val = HAL_I2C_Slave_Transmit_IT(&iic_inst->handle_struct, msg, length);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn      IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
 * @brief   Function that read over IIC connection
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @param   slave_addr Adress of the slave to which the message will be requested
 * @param   msg Message we want to receive
 * @param   length Size of the message we want to receive
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling and interrupt mode
 * Needs to supports DMA
 */
halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {
        uint32_t test_val;
        if (iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE)
        {
            test_val = HAL_I2C_Master_Receive(&iic_inst->handle_struct, slave_addr, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_POLLING_SLAVE_DRIVE)
        {
            test_val = HAL_I2C_Slave_Receive(&iic_inst->handle_struct, msg, length, HAL_MAX_DELAY);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE)
        {
            test_val = HAL_I2C_Master_Receive_IT(&iic_inst->handle_struct, slave_addr, msg, length);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else if (iic_inst->drive_type == IIC_IT_SLAVE_DRIVE)
        {
            test_val = HAL_I2C_Slave_Receive_IT(&iic_inst->handle_struct, msg, length);
            if (test_val != HAL_OK)
            {
                return_value = FCT_ERROR;
            }
        }
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

// cppcheck-suppress constParameter
/**
 * @fn      IictIoctl(iicInst_t *iic_inst)
 * @brief   Function that allows to change parameters
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 * 
 * @attention This feature is not supported yet so it does nothing
 * @todo Function may modifiy iic_inst handle_struct or drive_type (master/slave)
 */
halStatus_t IictIoctl(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (iic_inst != NULL)
    {
        /* TO DO */
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn      IicClose(iicInst_t *iic_inst)
 * @brief   Function that desinit the IIC connection and puts defaults parameters
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 *
 * This function erase iic_inst
 */
halStatus_t IicClose(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
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
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

/**
 * @fn      IicEnableInterrupt(iicInst_t *iic_inst)
 * @brief   Function that enables interrupt if needed
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if IT is not available for this IIC
 */
static halStatus_t IicEnableInterrupt(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE || iic_inst->drive_type == IIC_IT_SLAVE_DRIVE)
    {
        if (iic_inst->iic_ref == I2C1)
        {
            HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        }
        else if (iic_inst->iic_ref == I2C2)
        {
            HAL_NVIC_SetPriority(I2C2_EV_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
        }
#if defined(STM32F411xE)
        else if (iic_inst->iic_ref == I2C3)
        {
            HAL_NVIC_SetPriority(I2C3_EV_IRQn, 5, 0);
            HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
        }
#endif
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }

    return (return_value);
}

/**
 * @fn      IicDisableInterrupt(iicInst_t *iic_inst)
 * @brief   Function that disables interrupt if needed
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if IT is not available for this IIC
 */
static halStatus_t IicDisableInterrupt(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (iic_inst->drive_type == IIC_IT_MASTER_DRIVE || iic_inst->drive_type == IIC_IT_SLAVE_DRIVE)
    {
        if (iic_inst->iic_ref == I2C1)
        {
            HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
        }
        else if (iic_inst->iic_ref == I2C2)
        {
            HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
        }
#if defined(STM32F411xE)
        else if (iic_inst->iic_ref == I2C3)
        {
            HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
        }
#endif
        else
        {
            return_value = FCT_INVALID_PARAM;
        }
    }

    return (return_value);
}