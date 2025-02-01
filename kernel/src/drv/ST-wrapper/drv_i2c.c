/**
 * @file    drv_i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_i2c.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void I2cGenericIRQHandler(void *param);
static void I2cGenericDMAIRQHandler(void *param);
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst);
static returnCode_t I2cSetUpDMA(i2cInst_t *i2c_inst);
static returnCode_t I2cCheckRXTX(i2cInst_t *i2c_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              I2cOpen(i2cInst_t *i2c_inst)
 * @brief           Function that initialise a I2C connection
 * @param[in,out]   i2c_inst    Instance that contains I2C parameters and I2C Handler
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
        i2c_inst->handle_struct.Init.OwnAddress1 = 0u;
        i2c_inst->handle_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
        i2c_inst->handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
        i2c_inst->handle_struct.Init.OwnAddress2 = 0u;
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

        // Init I2C
        HAL_StatusTypeDef test_val = HAL_I2C_Init(&i2c_inst->handle_struct);
        if (test_val == HAL_OK)
        {
            // Setup DMA if necessary
            if (i2c_inst->driving_mode == DMA_MODE)
            {
                return_value = I2cSetUpDMA(i2c_inst);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Finally setup IRQ
                    return_value = I2cSetupIRQs(i2c_inst);
                }
            }
            else
            {
                // Finally setup IRQ
                return_value = I2cSetupIRQs(i2c_inst);
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
 * @fn          I2cWrite(i2cInst_t *i2c_inst, data_t data, length_t length)
 * @brief       Function that write over a I2C connection
 * @param[in]   i2c_inst    Instance that contains I2C parameters and I2C Handler
 * @param[in]   data        Message we want to send
 * @param[in]   length      Size of the message we want to sent
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if i2c timed out before sending message
 * @retval      #RET_NOT_AVAILABLE if i2c is still sending previous message
 */
returnCode_t I2cWrite(i2cInst_t *i2c_inst, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Write with driven mode
        if (i2c_inst->driving_mode == POLLING_MODE)
        {
            test_val = HAL_I2C_Master_Transmit(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length, DRV_MAX_DELAY);
        }
        else if (i2c_inst->driving_mode == INTERRUPT_MODE)
        {
            test_val = HAL_I2C_Master_Transmit_IT(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length);
        }
        else if (i2c_inst->driving_mode == DMA_MODE)
        {
            test_val = HAL_I2C_Master_Transmit_DMA(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length);
        }
        else
        {
            test_val = HAL_ERROR;
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
 * @fn          I2cRead(i2cInst_t *i2c_inst, data_t data, length_t length)
 * @brief       Function that read over I2C connection
 * @param[in]   i2c_inst    Instance that contains I2C parameters and I2C Handler
 * @param[out]  data        Message we want to receive
 * @param[in]   length      Size of the message we want to receive
 * @retval      #RET_SUCCESSFUL if message sent successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null
 * @retval      #RET_TIMEOUT if i2c timed out before receiving message
 * @retval      #RET_NOT_AVAILABLE if i2c is still receiving previous message
 */
returnCode_t I2cRead(i2cInst_t *i2c_inst, data_t data, length_t length)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Read with driven mode
        if (i2c_inst->driving_mode == POLLING_MODE)
        {
            test_val = HAL_I2C_Master_Receive(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length, DRV_MAX_DELAY);
        }
        else if (i2c_inst->driving_mode == INTERRUPT_MODE)
        {
            test_val = HAL_I2C_Master_Receive_IT(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length);
        }
        else if (i2c_inst->driving_mode == DMA_MODE)
        {
            test_val = HAL_I2C_Master_Receive_DMA(&i2c_inst->handle_struct, i2c_inst->slave_address, data, length);
        }
        else
        {
            test_val = HAL_ERROR;
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
 * @fn              I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   i2c_inst    Instance that contains I2C parameters and I2C Handler
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
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
        switch (cmd)
        {
        case IOCTL_PERIPHERAL_CHECK_RX:
        case IOCTL_PERIPHERAL_CHECK_TX:
            return_value = I2cCheckRXTX(i2c_inst);
            break;
        case IOCTL_I2C_SET_SLAVE_ADDRESS:
            if (data_size == sizeof(i2cSlaveAddr_t))
            {
                i2c_inst->slave_address = *(i2cSlaveAddr_t *)data;
            }
            else
            {
                return_value = RET_INVALID_PARAM;
            }
            break;
        default:
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
 * @fn              I2cClose(i2cInst_t *i2c_inst)
 * @brief           Function that desinit the I2C connection and puts defaults parameters
 * @param[in,out]   i2c_inst    Instance that contains I2C parameters and I2C Handler
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
 * @param[in]   i2c_inst    Instance that contains I2C parameters and I2C Handler
 * @retval      #RET_SUCCESSFUL if changing parameters succeed
 * @retval      #RET_INVALID_PARAM if IT is not available for this I2C
 */
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((i2c_inst->driving_mode == INTERRUPT_MODE) || (i2c_inst->driving_mode == DMA_MODE))
    {
        IRQHandlerParam_t param = (IRQHandlerParam_t)i2c_inst;
        return_value = RequestIRQ(i2c_inst->irq_no, 5u, I2cGenericIRQHandler, param);
    }

    return return_value;
}

/**
 * @fn          I2cSetUpDMA(i2cInst_t *i2c_inst)
 * @brief       Function that setup DMA if it exists
 * @param[in]   i2c_inst   Instance that contains I2C parameters and I2C Handler
 * @retval      #RET_SUCCESSFUL if changing parameters succeed
 * @retval      #RET_INVALID_PARAM if DMA is not available for this I2C
 */
returnCode_t I2cSetUpDMA(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;
    HAL_StatusTypeDef test_hal;

    // Function Core
    if (i2c_inst->driving_mode == DMA_MODE)
    {
        // First enable clock for DMA
        __HAL_RCC_DMA1_CLK_ENABLE();
        __HAL_RCC_DMA2_CLK_ENABLE();

        // Setup DMA RX
        i2c_inst->dma_rx_handle_struct.Instance = i2c_inst->dma_rx_ref;
#if defined(STM32H7)
        i2c_inst->dma_rx_handle_struct.Init.Request = i2c_inst->dma_rx_channel;
#elif defined(STM32F4)
        i2c_inst->dma_rx_handle_struct.Init.Channel = i2c_inst->dma_rx_channel;
#else
#error
#endif
        i2c_inst->dma_rx_handle_struct.Init.Direction = DMA_PERIPH_TO_MEMORY;
        i2c_inst->dma_rx_handle_struct.Init.PeriphInc = DMA_PINC_DISABLE;
        i2c_inst->dma_rx_handle_struct.Init.MemInc = DMA_MINC_ENABLE;
        i2c_inst->dma_rx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        i2c_inst->dma_rx_handle_struct.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        i2c_inst->dma_rx_handle_struct.Init.Mode = DMA_NORMAL;
        i2c_inst->dma_rx_handle_struct.Init.Priority = DMA_PRIORITY_LOW;
        i2c_inst->dma_rx_handle_struct.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        i2c_inst->dma_rx_handle_struct.Parent = &i2c_inst->handle_struct;
        i2c_inst->handle_struct.hdmarx = &i2c_inst->dma_rx_handle_struct;

        // Init DMA RX
        test_hal = HAL_DMA_Init(&i2c_inst->dma_rx_handle_struct);
        if (test_hal == HAL_OK)
        {
            // Setup DMA TX
            i2c_inst->dma_tx_handle_struct.Instance = i2c_inst->dma_tx_ref;
#if defined(STM32H7)
            i2c_inst->dma_tx_handle_struct.Init.Request = i2c_inst->dma_tx_channel;
#elif defined(STM32F4)
            i2c_inst->dma_tx_handle_struct.Init.Channel = i2c_inst->dma_tx_channel;
#else
#error Architecture is not supported
#endif
            i2c_inst->dma_tx_handle_struct.Init.Direction = DMA_MEMORY_TO_PERIPH;
            i2c_inst->dma_tx_handle_struct.Init.PeriphInc = DMA_PINC_DISABLE;
            i2c_inst->dma_tx_handle_struct.Init.MemInc = DMA_MINC_ENABLE;
            i2c_inst->dma_tx_handle_struct.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            i2c_inst->dma_tx_handle_struct.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            i2c_inst->dma_tx_handle_struct.Init.Mode = DMA_NORMAL;
            i2c_inst->dma_tx_handle_struct.Init.Priority = DMA_PRIORITY_LOW;
            i2c_inst->dma_tx_handle_struct.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
            i2c_inst->dma_tx_handle_struct.Parent = &i2c_inst->handle_struct;
            i2c_inst->handle_struct.hdmatx = &i2c_inst->dma_tx_handle_struct;

            // Init DMA TX
            test_hal = HAL_DMA_Init(&i2c_inst->dma_tx_handle_struct);
            if (test_hal == HAL_OK)
            {
                // Setup IRQ DMA RX
                IRQHandlerParam_t param = (IRQHandlerParam_t)&i2c_inst->dma_rx_handle_struct;
                return_value = RequestIRQ(i2c_inst->dma_rx_irq_no, 8u, I2cGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Setup IRQ DMA TX
                    param = (IRQHandlerParam_t)&i2c_inst->dma_tx_handle_struct;
                    return_value = RequestIRQ(i2c_inst->dma_tx_irq_no, 8u, I2cGenericDMAIRQHandler, param);
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
 * @fn              I2cCheckRXTX(i2cInst_t *i2c_inst, void *data)
 * @brief           Function that checks the status of a I2C reception and transmission
 * @param[in,out]   i2c_inst   Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if I2C is still receiving or transmitting data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t I2cCheckRXTX(i2cInst_t *i2c_inst)
{
    // Variable Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (i2c_inst != NULL)
    {
        if (i2c_inst->handle_struct.State == HAL_I2C_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if ((i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_RX) ||
                 (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_TX) ||
                 (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_RX_LISTEN) ||
                 (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_TX_LISTEN) ||
                 (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY) ||
                 (i2c_inst->handle_struct.State == HAL_I2C_STATE_LISTEN))
        {
            return_value = RET_NOT_AVAILABLE;
        }
        else if (i2c_inst->handle_struct.State == HAL_I2C_STATE_TIMEOUT)
        {
            return_value = RET_TIMEOUT;
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
 * @fn      I2cGenericIRQHandler(void *param)
 * @brief   Generic I2C Handler
 */
static void I2cGenericIRQHandler(void *param)
{
    // Get i2c inst
    i2cInst_t *i2c_inst = (i2cInst_t *)param;

    // Save pre-interrupt status
    HAL_I2C_StateTypeDef old_status = i2c_inst->handle_struct.State;

    // Do IRQ
    HAL_I2C_EV_IRQHandler(&i2c_inst->handle_struct);

    // Check if action has completed
    if ((i2c_inst->handle_struct.State == HAL_I2C_STATE_READY))
    {
        if ((old_status == HAL_I2C_STATE_BUSY_RX) || (old_status == HAL_I2C_STATE_BUSY_RX_LISTEN))
        {
            // RX completed
            if (i2c_inst->callback_rx_completed != NULL)
            {
                i2c_inst->callback_rx_completed(i2c_inst->callback_rx_completed_param);
            }
        }
        if ((old_status == HAL_I2C_STATE_BUSY_TX) || (old_status == HAL_I2C_STATE_BUSY_TX_LISTEN))
        {
            // TX completed
            if (i2c_inst->callback_tx_completed != NULL)
            {
                i2c_inst->callback_tx_completed(i2c_inst->callback_tx_completed_param);
            }
        }
    }
}

/**
 * @fn      I2cGenericDMAIRQHandler(void *param)
 * @brief   Generic I2C DMA Handler
 */
static void I2cGenericDMAIRQHandler(void *param)
{
    DMAHandleStruct_t *handle_struct = (DMAHandleStruct_t *)param;
    HAL_DMA_IRQHandler(handle_struct);
}