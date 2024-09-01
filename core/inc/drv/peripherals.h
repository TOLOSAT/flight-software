/**
 * @file    peripherals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining peripherals
 * @date    19/08/2024
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup peripherals Peripherals Management
 * Functions that allows to manage peripherals in TAPAS
 * @{
 */

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "sync/mutex.h"

#include "drv/drv_types.h"
#include "drv/drv_common.h"
#include "drv/drv_disk.h"
#include "drv/drv_ecc.h"
#include "drv/drv_gpio.h"
#include "drv/drv_i2c.h"
#include "drv/drv_ow.h"
#include "drv/drv_rtc.h"
#include "drv/drv_spi.h"
#include "drv/drv_tim.h"
#include "drv/drv_uart.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    peripheralType_t
 * @brief   Peripheral type typedef enum
 */
typedef enum
{
    PERIPHERALS_GPIO    = 0u,   /**< GPIO type peripheral */
    PERIPHERALS_UART    = 1u,   /**< UART type peripheral */
    PERIPHERALS_I2C     = 2u,   /**< I2C type peripheral */
    PERIPHERALS_SPI     = 3u,   /**< SPI type peripheral */
    PERIPHERALS_OW      = 4u,   /**< OW type peripheral */
} peripheralType_t;

/** @brief Peripheral Reference number type */
typedef uint32_t peripheralNo_t;

/** 
 * @struct  peripheralConf_t
 * @brief   Struct type of a peripheral configuration
 */
typedef struct
{
    void *p_conf;                   /**< @brief Pointer to the peripheral configuration */
    mutexQueue_t *p_mutex_queue;    /**< @brief Pointer to the peripheral mutex queue */
} peripheralConf_t;

/** 
 * @struct  peripheralDesc_t
 * @brief   Struct type of a peripheral descriptors
 */
typedef struct
{
    peripheralType_t type;  /**< @brief Peripheral type (GPIO, UART, I2C, ...) */
    void *p_instance;       /**< @brief Pointer to the peripheral instance */
    mutexHandle_t mutex;    /**< @brief Peripheral mutex */
} peripheralDesc_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t InitPeripherals(void);
extern coreStatus_t LockPeripherals(peripheralNo_t peripheral);
extern coreStatus_t UnlockPeripherals(peripheralNo_t peripheral);

#endif /* PERIPHERALS_H */

/** 
 * @} 
 * @} 
 */