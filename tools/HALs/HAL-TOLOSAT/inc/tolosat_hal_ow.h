/**
 * @file    tolosat_hal_ow.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_ow Driver One Wire
 * Functions that allows to drive One Wire independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef TOLOSAT_HAL_OW_H
#define TOLOSAT_HAL_OW_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"
#include "tolosat_hal_gpio.h"

/***************************** Macros Definitions ****************************/

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
    gpioInst_t gpio_handle_struct;  /**< @brief GPIO handle struct for One Wire */
} owInst_t;


/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t OwOpen(owInst_t *ow_isnt);
halStatus_t OwWrite(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length);
halStatus_t OwRead(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length);
halStatus_t OwIoctl(owInst_t *ow_isnt, halIoCtlCmd_t io_cmd);
halStatus_t OwClose(owInst_t *ow_isnt);

#endif /* TOLOSAT_HAL_OW_H */

/** 
 * @} 
 * @} 
 */