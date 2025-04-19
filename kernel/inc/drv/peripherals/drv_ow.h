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
    OW_OP_STATE_RESET                    = 0u,  /**< OW Operation reset state */
    OW_OP_STATE_PULL_DOWN                = 1u,  /**< OW Operation pull down state */
    OW_OP_STATE_PULL_DOWN_WAIT_WRITE_1   = 2u,  /**< OW Operation pull down wait write 1 state */
    OW_OP_STATE_PULL_DOWN_WAIT_WRITE_0   = 3u,  /**< OW Operation pull down wait write 0 state */
    OW_OP_STATE_PULL_DOWN_WAIT_READ      = 4u,  /**< OW Operation pull down wait read state */
    OW_OP_STATE_PULL_DOWN_WAIT_INIT      = 5u,  /**< OW Operation pull down wait init state */
    OW_OP_STATE_PULL_UP                  = 6u,  /**< OW Operation pull up state */
    OW_OP_STATE_PULL_UP_WAIT_WRITE_1     = 7u,  /**< OW Operation pull up wait write 1 state */
    OW_OP_STATE_PULL_UP_WAIT_WRITE_0     = 8u,  /**< OW Operation pull up wait write 0 state */
    OW_OP_STATE_PULL_UP_WAIT_READ_ANSWER = 9u,  /**< OW Operation pull up wait read answer state */
    OW_OP_STATE_PULL_UP_WAIT_INIT_ANSWER = 10u, /**< OW Operation pull up wait init answer state */
    OW_OP_STATE_READ                     = 11u, /**< OW Operation read state */
    OW_OP_STATE_WAIT_READ_COMPLETE       = 12u, /**< OW Operation wait read complete state */
    OW_OP_STATE_WAIT_INIT_COMPLETE       = 13u, /**< OW Operation wait init complete state */
    OW_OP_STATE_UPDATE                   = 14u  /**< OW Operation update state */
} owOpState_t;

/**
 * @struct  owInst_t
 * @brief   Struct type definition of a One Wire instance
 */
typedef struct
{
    /* OW GPIO, Timer and Interrupt */
    gpioInst_t gpio;       /**< @brief GPIO instance for One Wire */
    timerInst_t timer;     /**< @brief Timer instance for One Wire */
    timerRef_t *timer_ref; /**< @brief Timer reference (TIM1, TIM2, ...) */
    IRQNo_t irq_no;        /**< @brief OW related interrupt */
    /* Configuration Parameters */
    drivingMode_t driving_mode; /**< @brief OW driving mode */
    /* OW State */
    owState_t state; /**< @brief OW state mode */
    /* Current Operation */
    owOp_t current_op;     /**< @brief Current operation */
    owOpState_t op_state;  /**< @brief OW operation state */
    data_t p_op_data;      /**< @brief Pointer to operation data */
    length_t op_len;       /**< @brief Operation len */
    length_t op_index;     /**< @brief Current index in the data */
    length_t op_bit_index; /**< @brief Current bit index in the data */
    /* Callbacks */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
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