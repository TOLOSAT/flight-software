/**
 * @file    iridium_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Iridium Driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup iridium-drv Iridium Driver
 * @brief High level driver for the Iridium Transceiver.
 * @{
 */

#ifndef IRIDIUM_DRIVER_H
#define IRIDIUM_DRIVER_H

/******************************* Include Files *******************************/

#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

// Global defines
#define IRIDIUM_INST_STRING_MAX_SIZE       128u    /**< Maximum string size for iridium instance content */
#define IRIDIUM_SDB_TX_MSG_SIZE            340u    /**< Maximum size of a message Iridium Modem can transmit (MO) */
#define IRIDIUM_SDB_TX_MSG_SIZE_ASCII      "340\r" /**< Maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_SDB_TX_MSG_SIZE_ASCII_SIZE 4u      /**< Size of the maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_SDB_RX_MSG_SIZE            270u    /**< Maximum size of a message Iridium Modem can receive (MT) */
#define IRIDIUM_SDB_RX_MSG_SIZE_ASCII      "270\r" /**< Maximum size of a message Iridium Modem can receive (MT) but in ASCII*/
#define IRIDIUM_SDB_RX_MSG_SIZE_ASCII_SIZE 4u      /**< Size of the maximum size of a message Iridium Modem can transmit (MO) but in ASCII */
#define IRIDIUM_LENGTH_SIZE                2u      /**< Length field size for binary transfer */
#define IRIDIUM_CHECKSUM_SIZE              2u      /**< Checksum size for binary transfer */

// Register config
#define HW_CTRL_REG_BAUDRATE_MASK          0x000Fu /**< Baudrate settings bits mask*/
#define HW_CTRL_REG_BAUDRATE_POS           0u      /**< Baudrate settings bits position */
#define HW_CTRL_REG_DTR_MODE_MASK          0x0030u /**< DTR mode settings bits mask */
#define HW_CTRL_REG_DTR_MODE_POS           4u      /**< DTR mode settings bits position */
#define HW_CTRL_REG_CTRL_FLOW_MODE_MASK    0x01c0u /**< Control Flow settings bits mask */
#define HW_CTRL_REG_CTRL_FLOW_MODE_POS     6u      /**< Control Flow settings bits position */
#define HW_CTRL_REG_SBD_TIMEOUT_MASK       0x0E00u /**< SBD Timeout settings bit mask */
#define HW_CTRL_REG_SBD_TIMEOUT_POS        9u      /**< SBD Timeout settings bits position */
#define HW_CTRL_REG_QUIET_MASK             0x1000u /**< Quiet setting bit mask */
#define HW_CTRL_REG_QUIET_POS              12u     /**< Quiet setting bit position */
#define HW_CTRL_REG_VERBOSITY_MASK         0x2000u /**< Verbosity setting bit mask */
#define HW_CTRL_REG_VERBOSITY_POS          13u     /**< Verbosity setting bit position */
#define HW_CTRL_REG_ECHO_MODE_MASK         0x4000u /**< Echo Mode setting bit mask */
#define HW_CTRL_REG_ECHO_MODE_POS          14u     /**< Echo Mode setting bit position */
#define HW_CTRL_REG_RING_MODE_MASK         0x8000u /**< Ring Mode setting bit mask */
#define HW_CTRL_REG_RING_MODE_POS          15u     /**< Ring Mode setting bit position */

// Transceiver settings
#define IRIDIUM_9600_BPS                   (5u << HW_CTRL_REG_BAUDRATE_POS)       /**< Constant for 9600 baudrate */
#define IRIDIUM_19200_BPS                  (6u << HW_CTRL_REG_BAUDRATE_POS)       /**< Constant for 19200 baudrate */
#define IRIDIUM_38400_BPS                  (7u << HW_CTRL_REG_BAUDRATE_POS)       /**< Constant for 38400 baudrate */
#define IRIDIUM_57600_BPS                  (8u << HW_CTRL_REG_BAUDRATE_POS)       /**< Constant for 57600 baudrate */
#define IRIDIUM_115200_BPS                 (9u << HW_CTRL_REG_BAUDRATE_POS)       /**< Constant for 115200 baudrate */
#define IRIDIUM_DTR_OFF                    (0u << HW_CTRL_REG_DTR_MODE_POS)       /**< Constant for disabling dtr */
#define IRIDIUM_DTR_MODE_1                 (1u << HW_CTRL_REG_DTR_MODE_POS)       /**< Constant for dtr mode 1 */
#define IRIDIUM_DTR_MODE_2                 (2u << HW_CTRL_REG_DTR_MODE_POS)       /**< Constant for dtr mode 2 */
#define IRIDIUM_DTR_MODE_3                 (3u << HW_CTRL_REG_DTR_MODE_POS)       /**< Constant for dtr mode 3 */
#define IRIDIUM_HW_CTRL_FLOW_DISABLE       (0u << HW_CTRL_REG_CTRL_FLOW_MODE_POS) /**< Constant for disabling HW CTRL */
#define IRIDIUM_HW_CTRL_FLOW_RTS_CTS       (3u << HW_CTRL_REG_CTRL_FLOW_MODE_POS) /**< Constant for enabling HW CTRL with RTS and CTS */
#define IRIDIUM_SBD_TIMEOUT_INF            (0u << HW_CTRL_REG_SBD_TIMEOUT_POS)    /**< Constant for setting SBD infinite timeout  */
#define IRIDIUM_SBD_TIMEOUT_2S             (2u << HW_CTRL_REG_SBD_TIMEOUT_POS)    /**< Constant for setting SBD 2s timeout */
#define IRIDIUM_QUIET_ON                   (1u << HW_CTRL_REG_VERBOSITY_POS)      /**< Constant for enabling quiet mode */
#define IRIDIUM_QUIET_OFF                  (0u << HW_CTRL_REG_VERBOSITY_POS)      /**< Constant for disabling quiet mode */
#define IRIDIUM_VERBOSE_ON                 (1u << HW_CTRL_REG_VERBOSITY_POS)      /**< Constant for enabling verbose mode */
#define IRIDIUM_VERBOSE_OFF                (0u << HW_CTRL_REG_VERBOSITY_POS)      /**< Constant for disabling verbose mode */
#define IRIDIUM_ECHO_ON                    (1u << HW_CTRL_REG_ECHO_MODE_POS)      /**< Constant for enabling echo mode */
#define IRIDIUM_ECHO_OFF                   (0u << HW_CTRL_REG_ECHO_MODE_POS)      /**< Constant for disabling echo mode */
#define IRIDIUM_MSG_RX_ALERT_ON            (1u << HW_CTRL_REG_RING_MODE_POS)      /**< Constant for enabling alert when an RX message is received */
#define IRIDIUM_MSG_RX_ALERT_OFF           (0u << HW_CTRL_REG_RING_MODE_POS)      /**< Constant for disabling alert when an RX message is received*/

/***************************** Types Definitions *****************************/

/**
 * @brief Iridium HW Control register
 * - bits [0..3]  : Baudrate
 * - bits [4..5]  : Data Terminal Ready Mode
 * - bits [6..8]  : Control Flow Mode
 * - bits [9..11] : Timeout for SBD
 * - bit 12       : Quiet
 * - bit 13       : Verbosity
 * - bit 14       : Echo Mode
 * - bit 15       : Ring Mode
 */
typedef uint16_t iridiumHwCtrlReg_t;

/** @brief Iridium SDB TX (MO) message type definition */
typedef uint8_t iridiumSDBTxMsg_t[IRIDIUM_SDB_TX_MSG_SIZE];

/** @brief Iridium SDB RX (MT) message type definition */
typedef uint8_t iridiumSDBRxMsg_t[IRIDIUM_SDB_RX_MSG_SIZE];

/**
 * @enum    iridiumSBDMessagePresence_t
 * @brief   Enum typedef for iridium sbd message presence (in buffer)
 */
typedef enum
{
    IRIDIUM_SBD_MSG_NOT_PRESENT = 0u,
    IRIDIUM_SBD_MSG_PRESENT     = 1u,
} iridiumSBDMessagePresence_t;

/**
 * @enum    iridiumSBDRingAlertStatus_t
 * @brief   Enum typedef for iridium ring alert status
 */
typedef enum
{
    IRIDIUM_NO_RING_ALERT = 0u,
    IRIDIUM_RING_ALERT    = 1u,
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
    IRIDIUM_TRANSCEIVER_OFF,   /**< Iridium transceiver is off */
    IRIDIUM_TRANSCEIVER_READY, /**< Iridium transceiver is ready */
    IRIDIUM_TRANSCEIVER_BUSY,  /**< Iridium transceiver is busy */
    IRIDIUM_TRANSCEIVER_ERROR, /**< Iridium transceiver has encountered an error */
} iridiumTransceiverState_t;

/**
 * @struct  iridiumSBDStatus_t
 * @brief   Struct type definition of a Iridium SBD Status
 */
typedef struct
{
    iridiumSBDMessagePresence_t tx_message_presence; /**< If a message is in the TX buffer or not */
    uint16_t tx_message_sequence_nb;                 /**< Sequence Number of the message in TX buffer */
    iridiumSBDMessagePresence_t rx_message_presence; /**< If a message is in the RX buffer or not */
    uint16_t rx_message_sequence_nb;                 /**< Sequence Number of the message in RX buffer */
    iridiumSBDRingAlertStatus_t ring_alert_status;   /**< Indicates if a ring alert has been received or not */
    uint16_t nb_rx_message;                          /**< Number of RX message are in the buffer */
} iridiumSBDStatus_t;

/**
 * @enum    iridiumTXSessionStatus_t
 * @brief   Type type definition of a Iridium Mobile Originated Session Status
 */
typedef enum
{
    IRIDIUM_MO_STATUS_NO_ERROR                          = 0u,  /**< MO message (if any) transferred successfully. */
    IRIDIUM_MO_STATUS_NO_ERROR_MT_TOO_BIG               = 1u,  /**< MO message transferred successfully, but message was too big to be delivered. */
    IRIDIUM_MO_STATUS_NO_ERROR_LOCATION_UPDATE_REJECTED = 2u,  /**< MO message transferred successfully, but location update was rejected. */
    IRIDIUM_MO_STATUS_RESERVED_SUCCESS_3                = 3u,  /**< Reserved, but indicates MO session success if used. */
    IRIDIUM_MO_STATUS_RESERVED_SUCCESS_4                = 4u,  /**< Reserved, but indicates MO session success if used. */
    IRIDIUM_MO_STATUS_CALL_TIMEOUT                      = 10u, /**< GSS reported that the call did not complete in the allowed time. */
    IRIDIUM_MO_STATUS_GSS_QUEUE_FULL                    = 11u, /**< MO message queue at the GSS is full. */
    IRIDIUM_MO_STATUS_TOO_MANY_SEGMENTS                 = 12u, /**< MO message has too many segments. */
    IRIDIUM_MO_STATUS_SESSION_INCOMPLETE                = 13u, /**< GSS reported that the session did not complete. */
    IRIDIUM_MO_STATUS_INVALID_SEGMENT_SIZE              = 14u, /**< Invalid segment size. */
    IRIDIUM_MO_STATUS_ACCESS_DENIED                     = 15u, /**< Access is denied. */
    IRIDIUM_MO_STATUS_ISU_LOCKED                        = 16u, /**< ISU has been locked and may not make SBD calls (see +CULK command). */
    IRIDIUM_MO_STATUS_GATEWAY_NOT_RESPONDING            = 17u, /**< Gateway not responding (local session timeout). */
    IRIDIUM_MO_STATUS_CONNECTION_LOST                   = 18u, /**< Connection lost (RF drop). */
    IRIDIUM_MO_STATUS_LINK_FAILURE                      = 19u, /**< Link failure (A protocol error caused termination of the call). */
    IRIDIUM_MO_STATUS_NO_NETWORK_SERVICE                = 32u, /**< No network service, unable to initiate call. */
    IRIDIUM_MO_STATUS_ANTENNA_FAULT                     = 33u, /**< Antenna fault, unable to initiate call. */
    IRIDIUM_MO_STATUS_RADIO_DISABLED                    = 34u, /**< Radio is disabled, unable to initiate call. */
    IRIDIUM_MO_STATUS_ISU_BUSY                          = 35u, /**< ISU is busy, unable to initiate call. */
    IRIDIUM_MO_STATUS_TRY_LATER_REGISTRATION            = 36u, /**< Try later, must wait 3 minutes since last registration. */
    IRIDIUM_MO_STATUS_SBD_TEMP_DISABLED                 = 37u, /**< SBD service is temporarily disabled. */
    IRIDIUM_MO_STATUS_TRY_LATER_TRAFFIC_MANAGEMENT      = 38u, /**< Try later, traffic management period (see +SBDLOE command). */
    IRIDIUM_MO_STATUS_BAND_VIOLATION                    = 64u, /**< Band violation (attempt to transmit outside permitted frequency band). */
    IRIDIUM_MO_STATUS_PLL_LOCK_FAILURE                  = 65u, /**< PLL lock failure; hardware error during attempted transmit. */
} iridiumTXSessionStatus_t;

/**
 * @enum    iridiumRXSessionStatus_t
 * @brief   Type type definition of a Iridium RX Session Status
 */
typedef enum
{
    IRIDIUM_MT_STATUS_NO_MESSAGE = 0, /**< No SBD message to receive from the GSS. */
    IRIDIUM_MT_STATUS_RECEIVED   = 1, /**< SBD message successfully received from the GSS. */
    IRIDIUM_MT_STATUS_ERROR      = 2, /**< An error occurred while attempting to perform a mailbox check or receive a message from the GSS. */
} iridiumRXSessionStatus_t;

/**
 * @struct  iridiumSBDSessionStatus_t
 * @brief   Struct type definition of a Iridium SBD Session Status
 */
typedef struct
{
    iridiumTXSessionStatus_t tx_session_status; /**< TX session status */
    uint16_t tx_message_sequence_nb;            /**< Sequence number of the message in TX buffer */
    iridiumRXSessionStatus_t rx_session_status; /**< TX session status */
    uint16_t rx_message_sequence_nb;            /**< Sequence number of the message in RX buffer */
    uint16_t rx_message_length;                 /**< Size of the message in RX buffer */
    uint16_t nb_rx_message;                     /**< Number messages in RX buffer */
} iridiumSBDSessionStatus_t;

/**
 * @struct  iridiumInst_t
 * @brief   Struct type definition of a Iridium instance
 */
typedef struct
{
    deviceNo_t dev_uart;                               /**< UART device that will be used for Iridium (need to be initialise outside the driver) */
    iridiumHwCtrlReg_t hw_ctrl_reg;                    /**< Iridium hardware control register (used to setup the transceiver) */
    char serial_number[IRIDIUM_INST_STRING_MAX_SIZE];  /**< Iridium transceiver serial number */
    iridiumTransceiverState_t iridium_state;           /**< Current status of the iridium transceiver */
    iridiumNetworkAvailability_t minimum_availability; /**< Minimum availability required to transmit data */
} iridiumInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t IridiumStart(iridiumInst_t *iridium_inst);
extern returnCode_t IridiumSendSDB(iridiumInst_t *iridium_inst, iridiumSDBTxMsg_t tx_msg);
extern returnCode_t IridiumReceiveSDB(iridiumInst_t *iridium_inst, iridiumSDBRxMsg_t rx_msg);
extern returnCode_t IridiumStop(iridiumInst_t *iridium_inst);

#endif /* IRIDIUM_DRIVER_H */

/**
 * @}
 * @}
 */