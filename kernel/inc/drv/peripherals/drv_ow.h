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
 * @enum    owState_t
 * @brief   OW state type enum
 */
typedef enum
{
    OW_STATE_NO_INIT      = 0u, /**< OW peripheral not initialised */
    OW_STATE_READY        = 1u, /**< OW peripheral ready */
    OW_STATE_BUSY_INIT_CO = 2u, /**< OW peripheral busy initialising connection */
    OW_STATE_BUSY_RX      = 3u, /**< OW peripheral busy receiving */
    OW_STATE_BUSY_TX      = 4u, /**< OW peripheral busy transmitting */
    OW_STATE_ERROR        = 5u, /**< OW peripheral encountered an error */
} owState_t;

/**
 * @enum    owOp_t
 * @brief   OW operation type enum
 */
typedef enum
{
    OW_NO_OP,      /** OW no current operation */
    OW_OP_INIT_CO, /** OW initialise connection operation */
    OW_OP_RX,      /** OW read operation */
    OW_OP_TX,      /** OW write operation */
} owOp_t;

/**
 * @enum    owOpState_t
 * @brief   OW operation state type enum
 */
typedef enum
{
    OW_OP_STATE_RESET        = 0u, /**< OW Operation reset state */
    OW_OP_STATE_PULL_DOWN    = 1u, /**< OW Operation pull down state */
    OW_OP_STATE_PULL_UP      = 2u, /**< OW Operation pull up state */
    OW_OP_STATE_READ         = 3u, /**< OW Operation read state */
    OW_OP_STATE_BIT_COMPLETE = 4u, /**< OW Operation update state */
} owOpState_t;

/**
 * @struct  owConf_t
 * @brief   Struct type definition of a One Wire configuration
 */
typedef struct
{
    /* Peripheral and IRQ numero */
    gpioPort_t *gpio_port; /**< @brief GPIO port reference for OW */
    gpioPin_t gpio_pin;    /**< @brief GPIO pin reference for OW */
    timerPeriph_t *timer;  /**< @brief Pointer to timer peripheral (TIM1, TIM2, ...) */
    IRQNo_t irq_no;        /**< @brief OW related interrupt */
    /* Configuration Parameters */
    drivingMode_t default_mode; /**< @brief OW driving mode */
} owConf_t;

/**
 * @struct  owHandle_t
 * @brief   Struct type definition of a OneWire handler
 */
typedef struct
{
    owState_t state;       /**< @brief OW state mode */
    bool presence;         /**< @brief Indicates if a device is on the bus */
    owOp_t current_op;     /**< @brief Current operation */
    owOpState_t op_state;  /**< @brief OW operation state */
    data_t p_op_data;      /**< @brief Pointer to operation data */
    length_t op_len;       /**< @brief Operation len */
    length_t op_index;     /**< @brief Current index in the data */
    length_t op_bit_index; /**< @brief Current bit index in the data */
} owHandle_t;

/**
 * @struct  owInst_t
 * @brief   Struct type definition of a One Wire descriptor
 */
typedef struct
{
    const owConf_t *p_conf;                         /**< @brief Pointer to OW conf */
    owHandle_t handle;                              /**< @brief OW Handler */
    gpioInst_t gpio;                                /**< @brief GPIO instance for OW */
    timerInst_t timer;                              /**< @brief Timer instance for OW */
    drivingMode_t current_mode;                     /**< @brief OW driving mode */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
} owInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t OwOpen(owInst_t *ow_inst, const owConf_t *const ow_conf);
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