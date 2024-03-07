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
#define IRIDIUM_SDB_TX_MSG_SIZE_ASCII       "340\r"     /**< Maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_SDB_TX_MSG_SIZE_ASCII_SIZE  4u          /**< Size of the maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_SDB_RX_MSG_SIZE             270u        /**< Maximum size of a message Iridium Modem can receive (MT) */
#define IRIDIUM_SDB_RX_MSG_SIZE_ASCII       "270\r"     /**< Maximum size of a message Iridium Modem can receive (MT) but in ASCII*/
#define IRIDIUM_SDB_RX_MSG_SIZE_ASCII_SIZE  4u          /**< Size of the maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_CHECKSUM_SIZE               2u          /**< Checksum size for binary transfer */

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
 * - bits [9..11] : Reserved
 * - bit 12       : Quiet
 * - bit 13       : Verbosity
 * - bit 14       : Echo Mode
 * - bit 15       : Ring Mode
 */
typedef uint16_t iridiumHwCtrlReg_t;

/** @brief Iridium SDB TX (MO) message type definition */
typedef uint8_t iridiumSDBTxMsg_t[IRIDIUM_SDB_TX_MSG_SIZE + IRIDIUM_CHECKSUM_SIZE];

/** @brief Iridium SDB RX (MT) message type definition */
typedef uint8_t iridiumSDBRxMsg_t[IRIDIUM_SDB_RX_MSG_SIZE + IRIDIUM_CHECKSUM_SIZE];

/** 
 * @enum    iridiumSBDMessagePresence_t
 * @brief   Enum typedef for iridium sbd message presence (in buffer)
 */
typedef enum
{
    IRIDIUM_SBD_MSG_NOT_PRESENT = 0u,
    IRIDIUM_SBD_MSG_PRESENT = 1u,
} iridiumSBDMessagePresence_t;

/** 
 * @enum    iridiumSBDRingAlertStatus_t
 * @brief   Enum typedef for iridium ring alert status
 */
typedef enum
{
    IRIDIUM_NO_RING_ALERT = 0u,
    IRIDIUM_RING_ALERT = 1u,
} iridiumSBDRingAlertStatus_t;


/** 
 * @enum    iridiumNetworkAvailability_t
 * @brief   Enum typedef for iridium network availability
 */
typedef enum
{
    IRIDIUM_NETWORK_NO_SIGNAL,
    IRIDIUM_NETWORK_ALMOST_NO_SIGNAL,
    IRIDIUM_NETWORK_POOR,
    IRIDIUM_NETWORK_FAIR,
    IRIDIUM_NETWORK_GOOD,
    IRIDIUM_NETWORK_EXCELLENT,
} iridiumNetworkAvailability_t;

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
 * @struct  iridiumSBDStatus_t
 * @brief   Struct type definition of a Iridium SBD Status
 */
typedef struct
{
    iridiumNetworkAvailability_t network_availability;  /**< Iridium network availability */
    iridiumSBDMessagePresence_t tx_message_presence;    /**< If a message is in the TX buffer or not */
    uint16_t tx_message_sequence_nb;                    /**< Sequence Number of the message in TX buffer */
    iridiumSBDMessagePresence_t rx_message_presence;    /**< If a message is in the RX buffer or not */
    uint16_t rx_message_sequence_nb;                    /**< Sequence Number of the message in RX buffer */
    iridiumSBDRingAlertStatus_t ring_alert_status;      /**< Indicates if a ring alert has been received or not */
    uint16_t nb_rx_message;                             /**< Number of RX message are in the buffer */
} iridiumSBDStatus_t;

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
    iridiumNetworkAvailability_t minimum_availability;      /**< Minimum availability required to transmit data */
} iridiumInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

iridiumStatus_t IridiumStart(iridiumInst_t *iridium_inst);
iridiumStatus_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t *tx_msg);
// iridiumStatus_t IridiumReceiveSDB(iridiumInst_t *iridium_inst, iridiumSDBRxMsg_t *rx_msg);
iridiumStatus_t IridiumGetNetworkAvailability(iridiumInst_t *iridium_inst, iridiumNetworkAvailability_t *availability);
iridiumStatus_t IridiumGetSBDStatus(iridiumInst_t *iridium_inst, iridiumSBDStatus_t *status);
// iridiumStatus_t IridiumStop(iridiumInst_t *iridium_inst);

#endif /* IRIDIUM_DRIVER_H */

/** 
 * @} 
 */