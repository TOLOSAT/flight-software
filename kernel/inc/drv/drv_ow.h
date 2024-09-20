/**
 * @file    drv_ow.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_ow OneWire Driver
 * @brief Abstraction layer for controlling OneWire buses.
 * @{
 */

#ifndef DRV_OW_H
#define DRV_OW_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "drv/drv_gpio.h"
#include "drv/drv_tim.h"

/***************************** Macros Definitions ****************************/

#define OW_IOCTL_INIT_CONNECTION    0u  /**< OW IO CTL init connection */

/***************************** Types Definitions *****************************/

/** @brief One Wire message type definition */
typedef uint8_t owMsg_t;

/** @brief One Wire message length type definition */
typedef uint16_t owMsgLength_t;

/** 
 * @struct  owInst_t
 * @brief   Struct type definition of a One Wire instance
 */
typedef struct
{
    gpioInst_t  gpio_inst;      /**< @brief GPIO instance for One Wire */
    timerInst_t timer; /**< @brief Timer instance for One Wire */
} owInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern kernelStatus_t OwOpen(owInst_t *ow_inst);
extern kernelStatus_t OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length);
extern kernelStatus_t OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length);
extern kernelStatus_t OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size);
extern kernelStatus_t OwClose(owInst_t *ow_inst);

#endif /* DRV_OW_H */

/** 
 * @}
 * @}
 * @}
 */