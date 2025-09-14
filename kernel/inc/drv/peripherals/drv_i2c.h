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

/** @brief I2C peripheral type redefinition (I2C1, I2C2, ...) */
typedef I2C_TypeDef i2cPeriph_t;

/** @brief I2C slave addr type definition */
typedef uint16_t i2cSlaveAddr_t;

/**
 * @struct  i2cConf_t
 * @brief   Struct type definition of a I2C configuration
 */
typedef struct
{
    i2cPeriph_t *periph;        /**< @brief Pointer to the I2C peripheral (I2C1, I2C2, ...) */
    IRQNo_t irq_no;             /**< @brief I2C related interrupt */
    IRQPrio_t irq_prio;         /**< @brief I2C related interrupt priority */
    drivingMode_t default_mode; /**< @brief I2C driving mode */
    clockSource_t clk_src;      /**< @brief I2C peripheral clock source */
    DMAConf_t dma_tx;           /**< @brief I2C DMA configuration for TX */
    DMAConf_t dma_rx;           /**< @brief I2C DMA configuration for TX */
    IOConf_t io_scl;            /**< @brief I2C IO configuration for SCL */
    IOConf_t io_sda;            /**< @brief I2C IO configuration for SDA */
} i2cConf_t;

/**
 * @struct  i2cInst_t
 * @brief   Struct type definition of a I2C instance
 */
typedef struct
{
    i2cHandleStruct_t handle_struct;                /**< @brief I2C handle struct used by HAL */
    drivingMode_t current_mode;                     /**< @brief Current driving mode */
    drivingMode_t current_slave;                    /**< @brief Current driving mode */
    DMAHandleStruct_t dma_rx_handle_struct;         /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct;         /**< @brief DMA TX handle struct used by HAL */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
    const i2cConf_t *p_conf;                        /**< @brief Pointer to I2C conf */
} i2cInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t I2cOpen(i2cInst_t *i2c_inst, const i2cConf_t *const i2c_conf);
extern returnCode_t I2cWrite(i2cInst_t *i2c_inst, data_t msg, length_t length);
extern returnCode_t I2cRead(i2cInst_t *i2c_inst, data_t msg, length_t length);
extern returnCode_t I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t I2cClose(i2cInst_t *i2c_inst);

#endif /* DRV_I2C_H */

/**
 * @}
 * @}
 * @}
 */