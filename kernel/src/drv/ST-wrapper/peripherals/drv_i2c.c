/**
 * @file    drv_i2c.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals/drv_i2c.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void I2cGenericIRQHandler(void *param);
static void I2cGenericDMAIRQHandler(void *param);
static returnCode_t I2cInitClock(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf);
static returnCode_t I2cSetupIOs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf);
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf);
static returnCode_t I2cSetUpDMA(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf);
static returnCode_t I2cCheckRXTX(i2cInst_t *i2c_inst);
static returnCode_t I2cStopRXTX(i2cInst_t *i2c_inst);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              I2cOpen(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
 * @brief           Function that initialise a I2C connection
 * @param[in,out]   i2c_inst   Instance that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if I2C ref is not available for this board or one pointer is null
 */
returnCode_t I2cOpen(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((i2c_inst != NULL) && (i2c_conf != NULL))
    {
        // Init peripheral clock
        return_value = I2cInitClock(i2c_inst, i2c_conf);
        if (return_value == RET_SUCCESSFUL)
        {
            // Setup IOs
            return_value = I2cSetupIOs(i2c_inst, i2c_conf);
            if (return_value == RET_SUCCESSFUL)
            {
                // Setup I2C
                i2c_inst->handle_struct.Instance             = i2c_conf->i2c_ref;
                i2c_inst->handle_struct.Init.OwnAddress1     = 0u;
                i2c_inst->handle_struct.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
                i2c_inst->handle_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
                i2c_inst->handle_struct.Init.OwnAddress2     = 0u;
                i2c_inst->handle_struct.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
                i2c_inst->handle_struct.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
#if defined(STM32H7)
                i2c_inst->handle_struct.Init.Timing = 0x307075B1;
#elif defined(STM32F4)
                i2c_inst->handle_struct.Init.ClockSpeed = 100000;
                i2c_inst->handle_struct.Init.DutyCycle  = I2C_DUTYCYCLE_2;
#else
#error "Current STM32 familly is not supported"
#endif

                // Init I2C
                HAL_StatusTypeDef test_val = HAL_I2C_Init(&i2c_inst->handle_struct);
                if (test_val == HAL_OK)
                {
                    // Set current mode
                    i2c_inst->current_mode = i2c_conf->default_mode;
                    // Setup DMA if necessary
                    if (i2c_conf->default_mode == DMA_MODE)
                    {
                        return_value = I2cSetUpDMA(i2c_inst, i2c_conf);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Finally setup IRQ
                            return_value = I2cSetupIRQs(i2c_inst, i2c_conf);
                        }
                    }
                    else
                    {
                        // Finally setup IRQ
                        return_value = I2cSetupIRQs(i2c_inst, i2c_conf);
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((i2c_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Write with driven mode
        if (i2c_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_I2C_Master_Transmit(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length, DRV_MAX_DELAY);
        }
        else if (i2c_inst->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_I2C_Master_Transmit_IT(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length);
        }
        else if (i2c_inst->current_mode == DMA_MODE)
        {
            test_val = HAL_I2C_Master_Transmit_DMA(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length);
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((i2c_inst != NULL) && (data != NULL) && (length != 0u))
    {
        HAL_StatusTypeDef test_val;
        // Read with driven mode
        if (i2c_inst->current_mode == POLLING_MODE)
        {
            test_val = HAL_I2C_Master_Receive(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length, DRV_MAX_DELAY);
        }
        else if (i2c_inst->current_mode == INTERRUPT_MODE)
        {
            test_val = HAL_I2C_Master_Receive_IT(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length);
        }
        else if (i2c_inst->current_mode == DMA_MODE)
        {
            test_val = HAL_I2C_Master_Receive_DMA(&i2c_inst->handle_struct, i2c_inst->current_slave, data, length);
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (i2c_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_PERIPHERAL_CHECK_RX :
            case IOCTL_PERIPHERAL_CHECK_TX :
                return_value = I2cCheckRXTX(i2c_inst);
                break;
            case IOCTL_PERIPHERAL_STOP_RXTX :
            case IOCTL_PERIPHERAL_STOP_RX :
            case IOCTL_PERIPHERAL_STOP_TX :
                return_value = I2cStopRXTX(i2c_inst);
                break;
            case IOCTL_I2C_SET_SLAVE_ADDR :
                if (data_size == sizeof(i2cSlaveAddr_t))
                {
                    i2c_inst->current_slave = *(i2cSlaveAddr_t *)data;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
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
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (i2c_inst != NULL)
    {
        HAL_I2C_DeInit(&i2c_inst->handle_struct);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              I2cInitClock(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
 * @brief           Function that setups I2C peripheral clock
 * @param[in,out]   i2c_inst   Instance that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_ERROR if the clock initialisation failed
 */
static returnCode_t I2cInitClock(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(i2c_inst);

    // Check parameter(s)
    if ((i2c_inst != NULL) && (i2c_conf != NULL))
    {
        // Select the peripheral clock
        switch ((uintptr_t)i2c_conf->i2c_ref)
        {
            case I2C1_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef i2c_peripheral_clock_settings = { 0 };
                i2c_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_I2C1;
                i2c_peripheral_clock_settings.I2c123ClockSelection     = i2c_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&i2c_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_I2C1_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_I2C1_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#if defined(I2C2)
            case I2C2_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef i2c_peripheral_clock_settings = { 0 };
                i2c_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_I2C2;
                i2c_peripheral_clock_settings.I2c123ClockSelection     = i2c_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&i2c_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_I2C2_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_I2C2_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* I2C2 */
#if defined(I2C3)
            case I2C3_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef i2c_peripheral_clock_settings = { 0 };
                i2c_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_I2C3;
                i2c_peripheral_clock_settings.I2c123ClockSelection     = i2c_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&i2c_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_I2C3_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_I2C3_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* I2C3 */
#if defined(I2C4)
            case I2C4_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef i2c_peripheral_clock_settings = { 0 };
                i2c_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_I2C4;
                i2c_peripheral_clock_settings.I2c4ClockSelection       = i2c_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&i2c_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_I2C4_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_I2C4_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* I2C4 */
#if defined(I2C5)
            case I2C5_BASE :
            {
#if defined(STM32H7)
                RCC_PeriphCLKInitTypeDef i2c_peripheral_clock_settings = { 0 };
                i2c_peripheral_clock_settings.PeriphClockSelection     = RCC_PERIPHCLK_I2C5;
                i2c_peripheral_clock_settings.I2c1235ClockSelection    = i2c_conf->clk_src;
                if (HAL_RCCEx_PeriphCLKConfig(&i2c_peripheral_clock_settings) == HAL_OK)
                {
                    __HAL_RCC_I2C5_CLK_ENABLE();
                }
                else
                {
                    return_value = RET_ERROR;
                }
#elif defined(STM32F4)
                __HAL_RCC_I2C5_CLK_ENABLE();
#else
#error
#endif /* STM32H7 | STM32F4 */
                break;
            }
#endif /* I2C5 */
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
 * @fn              I2cSetupIOs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
 * @brief           Function that setups IOs
 * @param[in,out]   i2c_inst   Instance that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this I2C
 */
static returnCode_t I2cSetupIOs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Unused
    (void)(i2c_inst);

    // Check parameter(s)
    if ((i2c_inst != NULL) && (i2c_conf != NULL))
    {
        // First init SCL IO
        return_value = SetupIO(&i2c_conf->io_scl);
        if (return_value == RET_SUCCESSFUL)
        {
            // Then init SDA IO
            return_value = SetupIO(&i2c_conf->io_sda);
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              I2cSetupIRQs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
 * @brief           Function that setups interrupt if needed
 * @param[in,out]   i2c_inst   Instance that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if IT is not available for this I2C
 */
static returnCode_t I2cSetupIRQs(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((i2c_inst != NULL) && (i2c_conf != NULL) && ((i2c_conf->default_mode == INTERRUPT_MODE) || (i2c_conf->default_mode == DMA_MODE)))
    {
        // Set i2c inst as the interrupt parameter to pass it to the interrupt routine
        IRQHandlerParam_t param = (IRQHandlerParam_t)i2c_inst;
        // Request the interrupt
        return_value = RequestIRQ(i2c_conf->irq_no, 5u, I2cGenericIRQHandler, param);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              I2cSetUpDMA(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
 * @brief           Function that setup DMA if it exists
 * @param[in,out]   i2c_inst   Instance that contains I2C handlers
 * @param[in]       i2c_conf   Configuration that contains I2C parameters
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if DMA is not available for this I2C
 */
static returnCode_t I2cSetUpDMA(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((i2c_inst != NULL) && (i2c_conf != NULL) && (i2c_conf->default_mode == DMA_MODE))
    {
        // First Setup RX DMA
        return_value = SetUpDMA(&i2c_inst->dma_rx_handle_struct, &i2c_conf->dma_rx);
        // Setup parents / children
        i2c_inst->dma_rx_handle_struct.Parent = &i2c_inst->handle_struct;
        i2c_inst->handle_struct.hdmarx        = &i2c_inst->dma_rx_handle_struct;
        if (return_value == RET_SUCCESSFUL)
        {
            // First Setup RX DMA
            return_value = SetUpDMA(&i2c_inst->dma_tx_handle_struct, &i2c_conf->dma_tx);
            // Setup parents / children
            i2c_inst->dma_tx_handle_struct.Parent = &i2c_inst->handle_struct;
            i2c_inst->handle_struct.hdmatx        = &i2c_inst->dma_tx_handle_struct;
            if (return_value == RET_SUCCESSFUL)
            {
                // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                IRQHandlerParam_t param = (IRQHandlerParam_t)&i2c_inst->dma_rx_handle_struct;
                // Request DMA RX interrupt
                return_value = RequestIRQ(i2c_conf->dma_rx.irq_no, 8u, I2cGenericDMAIRQHandler, param);
                if (return_value == RET_SUCCESSFUL)
                {
                    // Set DMA handle struct as the interrupt parameter to pass it to the interrupt routine
                    param = (IRQHandlerParam_t)&i2c_inst->dma_tx_handle_struct;
                    // Request DMA TX interrupt
                    return_value = RequestIRQ(i2c_conf->dma_tx.irq_no, 8u, I2cGenericDMAIRQHandler, param);
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
 * @fn              I2cCheckRXTX(i2cInst_t *i2c_inst)
 * @brief           Function that checks the status of a I2C reception and transmission
 * @param[in,out]   i2c_inst   Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if I2C is still receiving or transmitting data
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t I2cCheckRXTX(i2cInst_t *i2c_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (i2c_inst != NULL)
    {
        if (i2c_inst->handle_struct.State == HAL_I2C_STATE_READY)
        {
            return_value = RET_SUCCESSFUL;
        }
        else if ((i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_RX) || (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_TX)
                 || (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_RX_LISTEN) || (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY_TX_LISTEN)
                 || (i2c_inst->handle_struct.State == HAL_I2C_STATE_BUSY) || (i2c_inst->handle_struct.State == HAL_I2C_STATE_LISTEN))
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

/**
 * @fn              I2cStopRXTX(i2cInst_t *i2c_inst)
 * @brief           Function that stop the I2C reception and transmission
 * @param[in,out]   i2c_inst   Instance that contains I2C parameters and I2C Handler
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_SUCCESSFUL else
 */
static returnCode_t I2cStopRXTX(i2cInst_t *i2c_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (i2c_inst != NULL)
    {
        HAL_StatusTypeDef test_val = HAL_OK;
        // Read with driven mode
        if ((i2c_inst->current_mode == DMA_MODE) || (i2c_inst->current_mode == INTERRUPT_MODE))
        {
            test_val = HAL_I2C_Master_Abort_IT(&i2c_inst->handle_struct, i2c_inst->current_slave);
        }
        else if (i2c_inst->current_mode == POLLING_MODE)
        {
            // Nothing to abort
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
 * @fn      I2cGenericIRQHandler(void *param)
 * @brief   Generic I2C IRQ Handler
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
 * @brief   Generic I2C DMA IRQ Handler
 */
static void I2cGenericDMAIRQHandler(void *param)
{
    DMAHandleStruct_t *handle_struct = (DMAHandleStruct_t *)param;
    HAL_DMA_IRQHandler(handle_struct);
}