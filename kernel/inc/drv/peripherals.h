/**
 * @file    peripherals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining peripherals
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup peripherals Peripherals Management
 * @brief Abstraction layer for managing peripherals using their corresponding driver.
 * @{
 */

#ifndef PERIPHERALS_H
#define PERIPHERALS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/mutex.h"

#include "drv/drv_types.h"
#include "drv/drv_common.h"
#include "drv/drv_disk.h"
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

/** @brief Peripheral reference number type */
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

/**
 * @var     g_peripherals_conf_table
 * @brief   Configuration table where all peripherals configurations are stored
 */
extern peripheralConf_t g_peripherals_conf_table[NB_PERIPHERALS];

/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
extern peripheralDesc_t g_peripherals_desc_table[NB_PERIPHERALS];

/*************************** Functions Declarations **************************/

extern returnCode_t InitPeripherals(void);
extern returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info);
extern returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length, uint32_t extra_info);
extern returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t PeripheralLock(peripheralNo_t peripheral);
extern returnCode_t PeripheralUnlock(peripheralNo_t peripheral);

#endif /* PERIPHERALS_H */

/**
 * @}
 * @}
 * @}
 */