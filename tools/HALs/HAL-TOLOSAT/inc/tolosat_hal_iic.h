/**
 * @file tolosat_hal_iic.h
 * @author Merlin Kooshmanian
 * @brief Header file for TOLOSAT HAL IIC functions
 * @date 30/04/2023
 *
 * Last Update : 30/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_HAL_IIC_H
#define TOLOSAT_HAL_IIC_H

/***************************** Include Files *********************************/

#include "tolosat_hal_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef I2C_HandleTypeDef iicHandleStruct_t;
typedef I2C_TypeDef iicRef_t;
typedef uint16_t iicSlaveAddr_t;
typedef uint8_t iicMsg_t;
typedef uint16_t iicMsgLength_t;

typedef enum
{
    IIC_POLLING_MASTER_DRIVE = 0u, /**< IIC is driven in polling mode (CPU waits the data) and is bus master */
    IIC_POLLING_SLAVE_DRIVE = 1u,  /**< IIC is driven in polling mode (CPU waits the data) and is bus slave */
    IIC_IT_MASTER_DRIVE = 2u,      /**< IIC is driven by interrupts (CPU interrupts when there is data) and is bus master */
    IIC_IT_SLAVE_DRIVE = 3u,       /**< IIC is driven by interrupts (CPU interrupts when there is data) and is bus slave */
    IIC_DMA_MASTER_DRIVE = 4u,     /**< IIC is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus master */
    IIC_DMA_SLAVE_DRIVE = 5u,      /**< IIC is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus slave */
} iicDriveType_t;

typedef struct
{
    iicHandleStruct_t handle_struct;
    iicRef_t *iic_ref;
    iicDriveType_t drive_type;
    iicSlaveAddr_t own_address;
} iicInst_t;

/************************** Function Prototypes ******************************/

halStatus_t IicOpen(iicInst_t *iic_inst);
halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length);
halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length);
halStatus_t IictIoctl(iicInst_t *iic_inst);
halStatus_t IicClose(iicInst_t *iic_inst);

#endif /* TOLOSAT_HAL_IIC_H */