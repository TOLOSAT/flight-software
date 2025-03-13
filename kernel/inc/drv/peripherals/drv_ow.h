/**
 * @file    drv_ow.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for OW functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#include "drv/common.h"
#include "drv/peripherals/drv_gpio.h"
#include "drv/others/drv_tim.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  owInst_t
 * @brief   Struct type definition of a One Wire instance
 */
typedef struct
{
    gpioInst_t gpio_inst; /**< @brief GPIO instance for One Wire */
    timerInst_t timer;    /**< @brief Timer instance for One Wire */
} owInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t OwOpen(owInst_t *ow_inst);
extern returnCode_t OwWrite(owInst_t *ow_inst, data_t data, length_t length);
extern returnCode_t OwRead(owInst_t *ow_inst, data_t data, length_t length);
extern returnCode_t OwIoctl(owInst_t *ow_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t OwClose(owInst_t *ow_inst);

#endif /* DRV_OW_H */

/**
 * @}
 * @}
 * @}
 */