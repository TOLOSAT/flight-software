/**
 * @file    drv_i2c.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_i2c I2C Driver
 * @brief Abstraction layer for controlling I2C buses.
 * @{
 */

#ifndef DRV_I2C_H
#define DRV_I2C_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief I2C handle struct type redefinition */
typedef I2C_HandleTypeDef i2cHandleStruct_t;

/** @brief I2C reference type redefinition (I2C1, I2C2, ...) */
typedef I2C_TypeDef i2cRef_t;

/** @brief I2C slave addr type definition */
typedef uint16_t i2cSlaveAddr_t;

/**
 * @struct  i2cConf_t
 * @brief   Struct type definition of a I2C configuration
 */
typedef struct
{
    /* I2C Handle, Reference and Interrupt */
    i2cRef_t *i2c_ref; /**< @brief I2C reference (I2C1, I2C2, ...) */
    IRQNo_t irq_no;    /**< @brief I2C related interrupt */
    /* Configuration Parameters */
    drivingMode_t default_mode; /**< @brief I2C driving mode */
    /* DMA */
    DMARef_t *dma_rx_ref;        /**< @brief DMA RX reference (DMA1_Stream0, ...) */
    DMARef_t *dma_tx_ref;        /**< @brief DMA TX reference (DMA1_Stream0, ...) */
    DMAChannel_t dma_rx_channel; /**< @brief DMA RX related channel */
    DMAChannel_t dma_tx_channel; /**< @brief DMA TX related channel */
    IRQNo_t dma_rx_irq_no;       /**< @brief DMA RX interrupt */
    IRQNo_t dma_tx_irq_no;       /**< @brief DMA TX interrupt */
} i2cConf_t;

/**
 * @struct  i2cDesc_t
 * @brief   Struct type definition of a I2C descriptor
 */
typedef struct
{
    /* I2C Handle, Reference and Interrupt */
    i2cHandleStruct_t handle_struct; /**< @brief I2C handle struct used by HAL */
    drivingMode_t current_mode;      /**< @brief Current driving mode */
    drivingMode_t current_slave;     /**< @brief Current driving mode */
    /* DMA */
    DMAHandleStruct_t dma_rx_handle_struct; /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct; /**< @brief DMA TX handle struct used by HAL */
    /* Callbacks */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
} i2cDesc_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t I2cOpen(i2cDesc_t *i2c_desc, const i2cConf_t *const i2c_conf);
extern returnCode_t I2cWrite(i2cDesc_t *i2c_desc, data_t msg, length_t length);
extern returnCode_t I2cRead(i2cDesc_t *i2c_desc, data_t msg, length_t length);
extern returnCode_t I2cIoctl(i2cDesc_t *i2c_desc, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t I2cClose(i2cDesc_t *i2c_desc);

#endif /* DRV_I2C_H */

/**
 * @}
 * @}
 * @}
 */