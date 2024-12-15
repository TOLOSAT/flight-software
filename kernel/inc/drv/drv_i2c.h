/**
 * @file    drv_i2c.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for I2C functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

#include "drv/drv_types.h"
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
 * @enum    i2cDriveType_t
 * @brief   I2C driving mode type enum
 */
typedef enum
{
    I2C_POLLING_MASTER_DRIVE = 0u, /**< I2C is driven in polling mode (CPU waits the data) and is bus master */
    I2C_POLLING_SLAVE_DRIVE = 1u,  /**< I2C is driven in polling mode (CPU waits the data) and is bus slave */
    I2C_IT_MASTER_DRIVE = 2u,      /**< I2C is driven by interrupts (CPU interrupts when there is data) and is bus master */
    I2C_IT_SLAVE_DRIVE = 3u,       /**< I2C is driven by interrupts (CPU interrupts when there is data) and is bus slave */
    I2C_DMA_MASTER_DRIVE = 4u,     /**< I2C is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus master (not available) */
    I2C_DMA_SLAVE_DRIVE = 5u,      /**< I2C is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus slave (not available) */
} i2cDriveType_t;

/**
 * @struct  i2cInst_t
 * @brief   Struct type definition of a I2C instance
 */
typedef struct
{
    i2cHandleStruct_t handle_struct;    /**< @brief I2C handle struct used by ST HAL */
    i2cRef_t *i2c_ref;                  /**< @brief I2C reference (I2C1, I2C2, ...) */
    i2cDriveType_t drive_type;          /**< @brief I2C drive mode as defining in i2cDriveType_t enum */
    i2cSlaveAddr_t own_address;         /**< @brief I2C own address (when I2C instance is slave) */
    IRQNo_t irq_no;                     /**< @brief I2C related interrupt (IRQ_NONE if none) */
} i2cInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t I2cOpen(i2cInst_t *i2c_inst);
extern returnCode_t I2cWrite(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t msg, length_t length);
extern returnCode_t I2cRead(i2cInst_t *i2c_inst, i2cSlaveAddr_t slave_addr, data_t msg, length_t length);
extern returnCode_t I2cIoctl(i2cInst_t *i2c_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t I2cClose(i2cInst_t *i2c_inst);

#endif /* DRV_I2C_H */

/**
 * @}
 * @}
 * @}
 */