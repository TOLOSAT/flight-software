/**
 * @file    iridium_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Iridium Driver
 * @date    04/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup iridium-drv Iridium Driver
 * @{
 */

#ifndef IRIDIUM_DRIVER_H
#define IRIDIUM_DRIVER_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

#define IRIDIUM_INST_STRING_MAX_SIZE        128u        /**< Maximum string size for iridium instance content */
#define IRIDIUM_SDB_TX_MSG_SIZE             340u        /**< Maximum size of a message Iridium Modem can transmit (MO) */
#define IRIDIUM_SDB_RX_MSG_SIZE             270u        /**< Maximum size of a message Iridium Modem can receive (MT) */

#define HW_CTRL_REG_BAUDRATE_POS            0u          /**< Baudrate settings bits position */
#define HW_CTRL_REG_BAUDRATE_MASK           0x0007u     /**< Baudrate settings bits mask*/
#define HW_CTRL_REG_DTR_MODE_POS            3u          /**< DTR mode settings bits position */
#define HW_CTRL_REG_DTR_MODE_MASK           0x0018u     /**< DTR mode settings bits mask */
#define HW_CTRL_REG_CTRL_FLOW_MODE_POS      5u          /**< Control Flow settings bits position */
#define HW_CTRL_REG_CTRL_FLOW_MODE_MASK     0x00e0u     /**< Control Flow settings bits mask */
#define HW_CTRL_REG_RING_MODE_POS           14u         /**< Ring Mode setting bit position */
#define HW_CTRL_REG_RING_MODE_MASK          0x4000u     /**< Ring Mode setting bit mask */
#define HW_CTRL_REG_ECHO_MODE_POS           15u         /**< Echo Mode setting bit position */
#define HW_CTRL_REG_ECHO_MODE_MASK          0x8000u     /**< Echo Mode setting bit mask */

/***************************** Types Definitions *****************************/

/** 
 * @enum    iridiumStatus_t
 * @brief   Iridium functions specific returns 
 */
typedef enum
{
    IRIDIUM_SUCCESSFUL = 0u,    /**< Function succeed */
    IRIDIUM_ERROR = 1u,         /**< Function failed */
    IRIDIUM_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    IRIDIUM_TIMEOUT = 3u,       /**< Device timed out */
    IRIDIUM_BUSY = 4u,          /**< Device is busy */
} iridiumStatus_t;

/** 
 * @brief Iridium HW Control register
 * - bits [0..2]  : Baudrate
 * - bits [3..4]  : Data Terminal Ready Mode
 * - bits [5..7]  : Control Flow Mode
 * - bits [8..13] : Reserved
 * - bit 14       : Ring Mode
 * - bit 15       : Echo Mode
 */
typedef uint16_t iridiumHwCtrlReg_t;

/** @brief Iridium SDB TX (MO) message type definition */
typedef uint8_t iridiumSDBTxMsg_t[IRIDIUM_SDB_TX_MSG_SIZE];

/** @brief Iridium SDB RX (MT) message type definition */
typedef uint8_t iridiumSDBRxMsg_t[IRIDIUM_SDB_RX_MSG_SIZE];

/** 
 * @enum    iridiumTransceiverState_t
 * @brief   Enum typedef for iridium transceiver state
 */
typedef enum
{
    IRIDIUM_TRANSCEIVER_OFF,    /**< Iridium transceiver is off */
    IRIDIUM_TRANSCEIVER_READY,  /**< Iridium transceiver is ready */
    IRIDIUM_TRANSCEIVER_BUSY,   /**< Iridium transceiver is busy */
    IRIDIUM_TRANSCEIVER_ERROR,  /**< Iridium transceiver has encountered an error */
} iridiumTransceiverState_t;

/** 
 * @struct  iridiumInst_t
 * @brief   Struct type definition of a Iridium instance
 */
typedef struct
{
    uartInst_t *uart_inst;                                  /**< Pointer to the UART instance we use for Iridium */
    iridiumHwCtrlReg_t hw_ctrl_reg;                         /**< Iridium hardware control register (used to setup the transceiver) */
    char manufacturer_id[IRIDIUM_INST_STRING_MAX_SIZE];     /**< Iridium transceiver manufacturer id */
    char model_id[IRIDIUM_INST_STRING_MAX_SIZE];            /**< Iridium transceiver model id */
    char serial_number[IRIDIUM_INST_STRING_MAX_SIZE];       /**< Iridium transceiver serial number */
    iridiumTransceiverState_t iridium_state;                       /**< Current status of the iridium transceiver */
} iridiumInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

// iridiumStatus_t IridiumStart(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t *tx_msg);
// iridiumStatus_t IridiumReceiveSDB(iridiumInst_t *iridium_inst, iridiumSDBRxMsg_t *rx_msg);
// iridiumStatus_t IridiumGetConf(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumSetConf(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumStop(iridiumInst_t *iridium_inst);

#endif /* IRIDIUM_DRIVER_H */

/** 
 * @} 
 */