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
#include "mutex.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    peripheralType_t
 * @brief   Task Privilege
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
 * @struct  peripheralDesc_t
 * @brief   Struct type of a peripheral descriptors
 */
typedef struct
{
    peripheralType_t type;
    void *p_instance;
    mutexHandle_t mutex;
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