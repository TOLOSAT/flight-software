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
    uint32_t test_val = 0;

    // Function Core
    if (iic_inst != NULL)
    {
#if defined(STM32F411xE)
        if (iic_inst->iic_ref == I2C1 || iic_inst->iic_ref == I2C2 || iic_inst->iic_ref == I2C3)
#endif
#if defined(STM32F103xB)
            if (iic_inst->iic_ref == I2C1 || iic_inst->iic_ref == I2C2)
#endif
            {
                iic_inst->handle_struct.Instance = iic_inst->iic_ref;
                iic_inst->handle_struct.Init.ClockSpeed = 100000;
                iic_inst->handle_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
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
 * @param   msg Message we want to send
 * @param   length Size of the message we want to send
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling mode
 * Needs to supports IT and DMA
 */
halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    // Function Core
    if (iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {
        if (iic_inst->drive_type == IIC_POLLING_MASTER_DRIVE)
        {
            test_val = HAL_I2C_Master_Transmit(&iic_inst->handle_struct, slave_addr, msg, length, HAL_MAX_DELAY);
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
            test_val = HAL_I2C_Master_Transmit_IT(&iic_inst->handle_struct, slave_addr, msg, length);
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
 * @param   msg Message we want to receive
 * @param   length Size of the message we want to receive
 * @retval  FCT_SUCCESSFUL if message sent successfully
 * @retval  FCT_INVALID_PARAM if one pointer is null
 * @retval  FCT_ERROR if transmit went wrong
 *
 * Attention : currently works only in polling mode
 * Needs to supports IT and DMA
 */
halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;
    uint32_t test_val = 0;

    // Function Core
    if (iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {
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

/**
 * @fn      IictIoctl(iicInst_t *iic_inst)
 * @brief   Function that allows to change parameters
 * @param   iic_inst Instance that contains IIC parameters and IIC Handler
 * @retval  FCT_SUCCESSFUL if changing parameters succeed
 * @retval  FCT_INVALID_PARAM if instance is a null pointer
 *
 * This feature is not supported yet so it does nothing
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
        *iic_inst = null_inst;
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return (return_value);
}

/**************************************/
/******* MSP Function (ST only) *******/
/**************************************/
#if defined(STM32F411xE) || defined(STM32F103xB)

/**
 * @brief I2C MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (hi2c->Instance == I2C1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
#if defined(STM32F411xE)
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
#endif
#if defined(STM32F103xB)
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#endif
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
    }
}

/**
 * @brief I2C MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hi2c: I2C handle pointer
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_I2C1_CLK_DISABLE();

        /**I2C1 GPIO Configuration
        PB6     ------> I2C1_SCL
        PB7     ------> I2C1_SDA
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
    }
}

#endif