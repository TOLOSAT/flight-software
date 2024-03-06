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

// Global defines
#define IRIDIUM_INST_STRING_MAX_SIZE        128u        /**< Maximum string size for iridium instance content */
#define IRIDIUM_SDB_TX_MSG_SIZE             340u        /**< Maximum size of a message Iridium Modem can transmit (MO) */
#define IRIDIUM_SDB_RX_MSG_SIZE             270u        /**< Maximum size of a message Iridium Modem can receive (MT) */

// Register config
#define HW_CTRL_REG_BAUDRATE_MASK           0x000Fu     /**< Baudrate settings bits mask*/
#define HW_CTRL_REG_BAUDRATE_POS            0u          /**< Baudrate settings bits position */
#define HW_CTRL_REG_DTR_MODE_MASK           0x0030u     /**< DTR mode settings bits mask */
#define HW_CTRL_REG_DTR_MODE_POS            4u          /**< DTR mode settings bits position */
#define HW_CTRL_REG_CTRL_FLOW_MODE_MASK     0x01c0u     /**< Control Flow settings bits mask */
#define HW_CTRL_REG_CTRL_FLOW_MODE_POS      6u          /**< Control Flow settings bits position */
#define HW_CTRL_REG_QUIET_MASK              0x1000u     /**< Quiet setting bit mask */
#define HW_CTRL_REG_QUIET_POS               12u         /**< Quiet setting bit position */
#define HW_CTRL_REG_VERBOSITY_MASK          0x2000u     /**< Verbosity setting bit mask */
#define HW_CTRL_REG_VERBOSITY_POS           13u         /**< Verbosity setting bit position */
#define HW_CTRL_REG_ECHO_MODE_MASK          0x4000u     /**< Echo Mode setting bit mask */
#define HW_CTRL_REG_ECHO_MODE_POS           14u         /**< Echo Mode setting bit position */
#define HW_CTRL_REG_RING_MODE_MASK          0x8000u     /**< Ring Mode setting bit mask */
#define HW_CTRL_REG_RING_MODE_POS           15u         /**< Ring Mode setting bit position */

// Transceiver settings
#define IRIDIUM_9600_BPS                    (5u << HW_CTRL_REG_BAUDRATE_POS)        /**< Constant for 9600 baudrate */
#define IRIDIUM_19200_BPS                   (6u << HW_CTRL_REG_BAUDRATE_POS)        /**< Constant for 19200 baudrate */
#define IRIDIUM_115200_BPS                  (9u << HW_CTRL_REG_BAUDRATE_POS)        /**< Constant for 115200 baudrate */
#define IRIDIUM_DTR_OFF                     (0u << HW_CTRL_REG_DTR_MODE_POS)        /**< Constant for disabling dtr */
#define IRIDIUM_DTR_MODE_1                  (1u << HW_CTRL_REG_DTR_MODE_POS)        /**< Constant for dtr mode 1 */
#define IRIDIUM_DTR_MODE_2                  (2u << HW_CTRL_REG_DTR_MODE_POS)        /**< Constant for dtr mode 2 */
#define IRIDIUM_DTR_MODE_3                  (3u << HW_CTRL_REG_DTR_MODE_POS)        /**< Constant for dtr mode 3 */
#define IRIDIUM_HW_CTRL_FLOW_DISABLE        (0u << HW_CTRL_REG_CTRL_FLOW_MODE_POS)  /**< Constant for disabling HW CTRL */
#define IRIDIUM_HW_CTRL_FLOW_RTS_CTS        (3u << HW_CTRL_REG_CTRL_FLOW_MODE_POS)  /**< Constant for enabling HW CTRL with RTS and CTS */
#define IRIDIUM_QUIET_ON                    (1u << HW_CTRL_REG_VERBOSITY_POS)       /**< Constant for enabling quiet mode */
#define IRIDIUM_QUIET_OFF                   (0u << HW_CTRL_REG_VERBOSITY_POS)       /**< Constant for disabling quiet mode */
#define IRIDIUM_VERBOSE_ON                  (1u << HW_CTRL_REG_VERBOSITY_POS)       /**< Constant for enabling verbose mode */
#define IRIDIUM_VERBOSE_OFF                 (0u << HW_CTRL_REG_VERBOSITY_POS)       /**< Constant for disabling verbose mode */
#define IRIDIUM_ECHO_ON                     (1u << HW_CTRL_REG_ECHO_MODE_POS)       /**< Constant for enabling echo mode */
#define IRIDIUM_ECHO_OFF                    (0u << HW_CTRL_REG_ECHO_MODE_POS)       /**< Constant for disabling echo mode */
#define IRIDIUM_RING_ALERT_ON               (1u << HW_CTRL_REG_RING_MODE_POS)       /**< Constant for enabling ring alert */
#define IRIDIUM_RING_ALERT_OFF              (0u << HW_CTRL_REG_RING_MODE_POS)       /**< Constant for disabling ring alert */

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
 * - bits [0..3]  : Baudrate
 * - bits [4..5]  : Data Terminal Ready Mode
 * - bits [6..8]  : Control Flow Mode
 * - bits [9..12] : Reserved
 * - bit 13       : Verbosity
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
    IRIDIUM_TRANSCEIVER_INIT,   /**< Iridium transceiver is off */
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
    iridiumTransceiverState_t iridium_state;                /**< Current status of the iridium transceiver */
} iridiumInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

iridiumStatus_t IridiumStart(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t *tx_msg);
// iridiumStatus_t IridiumReceiveSDB(iridiumInst_t *iridium_inst, iridiumSDBRxMsg_t *rx_msg);
// iridiumStatus_t IridiumGetConf(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumSetConf(iridiumInst_t *iridium_inst);
// iridiumStatus_t IridiumStop(iridiumInst_t *iridium_inst);

#endif /* IRIDIUM_DRIVER_H */

/** 
 * @} 
 */