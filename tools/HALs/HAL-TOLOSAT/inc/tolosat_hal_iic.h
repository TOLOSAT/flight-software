/**
 * @file    tolosat_hal_iic.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL IIC functions
 * @date    30/04/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_iic Driver I2C
 * Functions that allows to drive I2C independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef TOLOSAT_HAL_IIC_H
#define TOLOSAT_HAL_IIC_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief I2C handle struct type redefinition */
typedef I2C_HandleTypeDef iicHandleStruct_t;

/** @brief I2C reference type redefinition (I2C1, I2C2, ...) */
typedef I2C_TypeDef iicRef_t;

/** @brief I2C slave addr type definition */
typedef uint16_t iicSlaveAddr_t;

/** @brief I2C message type definition */
typedef uint8_t iicMsg_t;

/** @brief I2C message length type definition */
typedef uint16_t iicMsgLength_t;

/** 
 * @enum    iicDriveType_t
 * @brief   I2C driving mode type enum
 */
typedef enum
{
    IIC_POLLING_MASTER_DRIVE = 0u, /**< IIC is driven in polling mode (CPU waits the data) and is bus master */
    IIC_POLLING_SLAVE_DRIVE = 1u,  /**< IIC is driven in polling mode (CPU waits the data) and is bus slave */
    IIC_IT_MASTER_DRIVE = 2u,      /**< IIC is driven by interrupts (CPU interrupts when there is data) and is bus master */
    IIC_IT_SLAVE_DRIVE = 3u,       /**< IIC is driven by interrupts (CPU interrupts when there is data) and is bus slave */
    IIC_DMA_MASTER_DRIVE = 4u,     /**< IIC is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus master (not available) */
    IIC_DMA_SLAVE_DRIVE = 5u,      /**< IIC is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus slave (not available) */
} iicDriveType_t;

/** 
 * @struct  iicInst_t
 * @brief   Struct type definition of a I2C instance
 */
typedef struct
{
    iicHandleStruct_t handle_struct;    /**< @brief I2C handle struct used by ST HAL */
    iicRef_t *iic_ref;                  /**< @brief I2C reference (I2C1, I2C2, ...) */
    iicDriveType_t drive_type;          /**< @brief I2C drive mode as defining in iicDriveType_t enum */
    iicSlaveAddr_t own_address;         /**< @brief I2C own address (when I2C instance is slave) */
} iicInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t IicOpen(iicInst_t *iic_inst);
halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length);
halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length);
halStatus_t IicIoctl(iicInst_t *iic_inst, halIoCtlCmd_t io_cmd);
halStatus_t IicClose(iicInst_t *iic_inst);

#endif /* TOLOSAT_HAL_IIC_H */

/** 
 * @} 
 * @} 
 */