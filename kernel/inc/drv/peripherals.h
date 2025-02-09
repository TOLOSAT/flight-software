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
    PERIPHERAL_GPIO = 0u, /**< GPIO type peripheral */
    PERIPHERAL_UART = 1u, /**< UART type peripheral */
    PERIPHERAL_I2C  = 2u, /**< I2C type peripheral */
    PERIPHERAL_SPI  = 3u, /**< SPI type peripheral */
    PERIPHERAL_OW   = 4u, /**< OW type peripheral */
} peripheralType_t;

/**
 * @enum    peripheralMode_t
 * @brief   Peripheral mode typedef enum
 */
typedef enum
{
    PERIPHERAL_SYNCHRONOUS  = 0u, /**< Mode asynchronous (e.g. polling) */
    PERIPHERAL_ASYNCHRONOUS = 1u, /**< Mode synchronous (e.g. interrupt or DMA) */
} peripheralMode_t;

/**
 * @enum    peripheralDataFlow_t
 * @brief   Peripheral TX-RX data flow typedef enum
 */
typedef enum
{
    PERIPHERAL_FLOW_COUPLED     = 0u, /**< TX and RX are coupled */
    PERIPHERAL_FLOW_INDEPENDENT = 1u, /**< TX and RX are independent */
} peripheralDataFlow_t;

/** @brief Peripheral reference number type */
typedef uint32_t peripheralNo_t;

/**
 * @struct  peripheralConf_t
 * @brief   Struct type of a peripheral configuration
 */
typedef struct
{
    peripheralType_t type;          /**< @brief Peripheral type (GPIO, UART, I2C, ...) */
    peripheralMode_t mode;          /**< @brief Peripheral mode (synchronous, asynchronous) */
    peripheralDataFlow_t data_flow; /**< @brief Peripheral data flow type (TX and RX coupled or independant) */
    mutexQueue_t *p_mutex_queue;    /**< @brief Pointer to the peripheral mutex queue */
    mutexQueue_t *p_rx_mutex_queue; /**< @brief Pointer to the peripheral receiving mutex queue */
    mutexQueue_t *p_tx_mutex_queue; /**< @brief Pointer to the peripheral transmitting mutex queue */
} peripheralConf_t;

/**
 * @struct  peripheralDesc_t
 * @brief   Struct type of a peripheral descriptors
 */
typedef struct
{
    void *p_instance;    /**< @brief Pointer to the peripheral instance */
    mutexHandle_t mutex; /**< @brief Peripheral mutex */
    struct
    {
        mutexHandle_t mutex; /**< @brief Peripheral receiving mutex */
        taskNo_t owner;      /**< @brief Peripheral receiving owner */
    } rx;                    /**< @brief Peripheral transmission sub-structure */
    struct
    {
        mutexHandle_t mutex; /**< @brief Peripheral transmitting mutex */
        taskNo_t owner;      /**< @brief Peripheral receiving owner */
    } tx;                    /**< @brief Peripheral reception sub-structure */
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

extern void InitPeripherals(void);
extern returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length);
extern returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length);
extern returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size);

#endif /* PERIPHERALS_H */

/**
 * @}
 * @}
 * @}
 */