/**
 * @file    peripherals.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining peripherals
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#include "drv/common.h"
#include "drv/peripherals/drv_gpio.h"
#include "drv/peripherals/drv_i2c.h"
#include "drv/peripherals/drv_ow.h"
#include "drv/peripherals/drv_spi.h"
#include "drv/peripherals/drv_uart.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     PERIPHERAL_CONF(peripheral_no)
 * @brief   Get peripheral conf from g_peripherals_conf_table
 */
#define PERIPHERAL_CONF(peripheral_no) (g_peripherals_conf_table[(peripheral_no) - 1u])

/**
 * @def     PERIPHERAL_DESC(peripheral_no)
 * @brief   Get peripheral conf from g_peripherals_desc_table
 */
#define PERIPHERAL_DESC(peripheral_no) (g_peripherals_desc_table[(peripheral_no) - 1u])

/**
 * @def     IS_A_VALID_PERIPHERAL(peripheral_no)
 * @brief   Indicates if the peripheral_no is valid
 */
#define IS_A_VALID_PERIPHERAL(peripheral_no)                                                                             \
    (((peripheral_no) != (peripheralNo_t)NO_PERIPHERAL) && ((peripheral_no) < (peripheralNo_t)CONFIG_MAX_NB_PERIPHERALS) \
     && (PERIPHERAL_DESC(peripheral_no).status == DESC_USED))

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
    peripheralNo_t peripheral;        /**< @brief Peripheral reference number */
    const void *const p_conf;         /**< @brief Pointer to the peripheral configuration */
    peripheralType_t type;            /**< @brief Peripheral type (GPIO, UART, I2C, ...) */
    peripheralMode_t synchronisation; /**< @brief Peripheral synchronisation (synchronous, asynchronous) */
    peripheralDataFlow_t flow_type;   /**< @brief Peripheral flow type (TX and RX coupled or independant) */
    mutexQueue_t *p_mutex_queue;      /**< @brief Pointer to the peripheral mutex queue */
    mutexQueue_t *p_rx_mutex_queue;   /**< @brief Pointer to the peripheral receiving mutex queue */
    mutexQueue_t *p_tx_mutex_queue;   /**< @brief Pointer to the peripheral transmitting mutex queue */
} peripheralConf_t;

/**
 * @struct  peripheralDesc_t
 * @brief   Struct type of a peripheral descriptors
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    void *p_desc;        /**< @brief Pointer to the peripheral descriptor */
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
extern peripheralConf_t g_peripherals_conf_table[CONFIG_MAX_NB_PERIPHERALS];

/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
extern peripheralDesc_t g_peripherals_desc_table[CONFIG_MAX_NB_PERIPHERALS];

/*************************** Functions Declarations **************************/

extern void InitPeripherals(void);
extern void CreatePeripheralsMutexes(void);
extern returnCode_t PeripheralWrite(peripheralNo_t peripheral, data_t data, length_t length);
extern returnCode_t PeripheralRead(peripheralNo_t peripheral, data_t data, length_t length);
extern returnCode_t PeripheralIoctl(peripheralNo_t peripheral, uint32_t cmd, void *data, uint32_t data_size);

#endif /* PERIPHERALS_H */

/**
 * @}
 * @}
 * @}
 */