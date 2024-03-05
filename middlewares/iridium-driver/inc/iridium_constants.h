/**
 * @file    iridium_constants.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for Iridium Constants
 * @date    04/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup iridium-drv Iridium Driver
 * @{
 */

#ifndef IRIDIUM_CONSTANTS_H
#define IRIDIUM_CONSTANTS_H

/*************************** Constants Definitions ***************************/

/**
 * @ref DTE Data Terminal Equipment 
 * Test
 */

/************************************/
/*********** BASIC ANSWERS **********/
/************************************/

/**
 * @def     AT_OK_ANSWER
 * @brief   OK answer 
 */
#define AT_OK_ANSWER                        "\r\nOK\r\n"

/************************************/
/********** BASIC COMMANDS *********/
/************************************/

/**
 * @def     AT_MSG_HEADER
 * @brief   Headers for an AT message 
 */
#define AT_CMD_EMPTY                        "AT\r"

/**
 * @def     AT_CMD_DISPLAY_REGS
 * @brief   Display all the S registers in the system.
 */
#define AT_CMD_DISPLAY_REGS                 "AT%R\r"

/**
 * @def     AT_CMD_FACTORY_RESET
 * @brief   Recall factory defaults.
 */
#define AT_CMD_FACTORY_RESET                "AT&F0\r"

/**
 * @def     AT_CMD_SET_FLOW_CTRL
 * @brief   Select the flow control method between the ISU and @ref DTE.
 * 
 * "n" must be :
 * - 0 : Disables flow control.
 * - 3 : Enables RTS/CTS flow control (default).
 * - 4 : Enables XON/XOFF flow control (not applicable to 9602/9602-SB/9603).
 * - 6 : Enables both RTS/CTS and XON/XOFF flow control (not applicable to 9602/9602-SB/9603).
 */
#define AT_CMD_SET_FLOW_CTRL                "AT&Kn\r"

/**
 * @def     AT_CMD_VIEW_CONF
 * @brief   View the current active configuration and stored profiles.
 */
#define AT_CMD_VIEW_CONF                    "AT&V\r"

/**
 * @def     AT_CMD_WRITE_CONF
 * @brief   Store the active profile in non-volatile memory.
 * 
 * "n" must be :
 * - 0 : Store current (active) configuration as profile 0.
 * - 1 : Store current (active) configuration as profile 1.
 */
#define AT_CMD_WRITE_CONF                   "AT&Wn\r"

/**
 * @def     AT_CMD_SET_DEFAULT_PROF
 * @brief   Designate Default Reset Profile.
 * 
 * "n" must be :
 * - 0 : Select profile 0 (default).
 * - 1 : Select profile 1.
 */
#define AT_CMD_SET_DEFAULT_PROF             "AT&Yn\r"

/**
 * @def     AT_CMD_FLUSH_EEPROM
 * @brief   Flush all pending writes to Eeprom.
 * @note    Shut down the radio, and prepare the Data Module to be powered down.
 */
#define AT_CMD_FLUSH_EEPROM                 "AT*F\r"

/**
 * @def     AT_CMD_SET_RADIO_ACTIVITY
 * @brief   Set the radio activity.
 * 
 * "n" must be :
 * - 0 : Disable radio activity.
 * - 1 : Enable radio activity (default).
 */
#define AT_CMD_SET_RADIO_ACTIVITY           "AT*Rn\r"

/**
 * @def     AT_CMD_ECHO
 * @brief   Echo command characters.
 * 
 * "n" must be :
 * - 0 : Characters are not echoed to the DTE.
 * - 1 : Characters are echoed to the DTE (default).
 */
#define AT_CMD_ECHO                         "ATEn\r"

/**
 * @def     AT_CMD_QUIET_MODE
 * @brief   Control ISU responses.
 * 
 * "n" must be :
 * - 0 : ISU responses are sent to the DTE (default).
 * - 1 : ISU responses are NOT sent to the DTE.
 */
#define AT_CMD_QUIET_MODE                   "ATQn\r"

/**
 * @def     AT_CMD_VERBOSE_MODE
 * @brief   Set the response format of the ISU, which may be either numeric or textual.
 * 
 * "n" must be :
 * - 0 : Numeric responses.
 * - 1 : Textual responses (default).
 */
#define AT_CMD_VERBOSE_MODE                 "ATVn\r"

/**
 * @def     AT_CMD_SOFT_RESET
 * @brief   Reset the ISU to a user-stored configuration.
 * 
 * "n" must be :
 * - 0 : Restores user configuration 0.
 * - 1 : Restores user configuration 1.
 */
#define AT_CMD_SOFT_RESET                   "ATZn\r"

/************************************/
/***** SHORT DATA BURST COMMANDS ****/
/************************************/

/**
 * @def     AT_CMD_SBD_INIT_SESSION
 * @brief   This command initiates an SBD session between the ISU and the ESS.
 */
#define AT_CMD_SBD_INIT_SESSION             "AT+SBDI\r"

/**
 * @def     AT_CMD_SBD_INIT_SESSION_EXTENDED
 * @brief   This command initiates an SBD session between the ISU and the GSS, setting the SBD Session Type.
 */
#define AT_CMD_SBD_INIT_SESSION_EXTENDED    "AT+SBDIX\r"

/**
 * @def     AT_CMD_SBD_SET_AUTO_REGISTR_MODE
 * @brief   Set the ISU’s Auto-registration SBD mode.
 * 
 * "n" must be :
 * - 0 : Disable automatic registration (default).
 * - 1 : Set the Auto-registration mode to “Automatic”.
 * - 2 : Set the Auto-registration mode to “Ask”.
 * - 3 : Set the Auto-registration mode to “Automatic” and enable report of <event>=3
 * - 4 : Set the Auto-registration mode to “Ask” and enable report of <event>=3
 */
#define AT_CMD_SBD_SET_AUTO_REGISTR_MODE    "AT+SBDAREG=n\r"

/**
 * @def     AT_CMD_SBD_SET_MAN_REGISTR_MODE
 * @brief   Triggers an SBD session to perform a manual SBD registration.
 * 
 * Must be followed by "=<location>", where <location> has format: [+|-]DDMM.MMM,[+|-]dddmm.mmm
 * - DD : Degrees latitude (00-89)
 * - MM : Minutes latitude (00-59) 
 * - MMM : Thousandths of minutes latitude (000-999)
 * - ddd : Degrees longitude (000-179)
 * - mm : Minutes longitude (00-59)
 * - mmm : Thousandths of minutes longitude (000-999)
 * The optional sign indicators specify latitude North (+) or South (-), and 
 * longitude East (+) or West (-). If omitted, the default is +.
 */
#define AT_CMD_SBD_SET_MAN_REGISTR_MODE     "AT+SBDREG=+DDMM.MMM,+DDDMM.MMM\r"

/**
 * @def     AT_CMD_SBD_READ_BIN_DATA
 * @brief   This command is used to read binary data from ISU.
 * 
 * The SBD message is transferred formatted as follows:
 * {2-byte message length} + {binary SBD message} + {2-byte checksum}
 */
#define AT_CMD_SBD_READ_BIN_DATA            "AT+SBDRB\r"

/**
 * @def     AT_CMD_SBD_READ_TEXT_DATA
 * @brief   Read a text message from the ISU.
 * 
 * Once the command is entered, the SBD message in the mobile terminated buffer is sent out of the port.
 * This command is similar to +SBDRB except no length or checksum will be provided.
 */
#define AT_CMD_SBD_READ_TEXT_DATA           "AT+SBDRT\r"

/**
 * @def     AT_CMD_SBD_WRITE_BIN_DATA
 * @brief   Write binary data to the ISU.
 * 
 * Must be followed by "=<SBD message length>", where The <SBD message length> 
 * parameter represents the length, in bytes, of the SBD message not including the 
 * mandatory two-byte checksum.
 * 
 * Once the command is entered, the ISU will indicate to the FA that it is prepared 
 * to receive the message by sending the ASCII encoded string “READY<CR><LF>” 
 * (hex 52 45 41 44 59 0D 0A) to the FA.
 */
#define AT_CMD_SBD_WRITE_BIN_DATA           "AT+SBDWB"

/**
 * @def     AT_CMD_SBD_WRITE_TEXT_DATA
 * @brief   Write a text message from the ISU.
 * 
 * Must be followed by "=<text message>", where the length of <text message> 
 * is limited to 120 bytes and the message is terminated when a carriage return 
 * is entered.
 */
#define AT_CMD_SBD_WRITE_TEXT_DATA          "AT+SBDWT"

/**
 * @def     AT_CMD_SBD_GATEWAY_DETACH
 * @brief   Initiates an SBD session to detach the ISU from the Gateway.
 */
#define AT_CMD_SBD_GATEWAY_DETACH           "AT+SBDDET\r"

/**
 * @def     AT_CMD_SBD_CLEAR_MSG_BUFFER
 * @brief   This command is used to clear the mobile originated buffer, mobile terminated buffer or both.
 * 
 * "n" must be :
 * - 0 : Clear the mobile originated buffer.
 * - 1 : Clear the mobile terminated buffer.
 * - 2 : Clear both the mobile originated and mobile terminated buffers.
 */
#define AT_CMD_SBD_CLEAR_MSG_BUFFER         "AT+SBDDn\r"

/**
 * @def     AT_CMD_SBD_GET_STATUS
 * @brief   Get SBD status.
 */
#define AT_CMD_SBD_GET_STATUS               "AT+SBDS\r"

/**
 * @def     AT_CMD_SBD_GET_STATUS_EXTENDED
 * @brief   Get SBD extended status.
 */
#define AT_CMD_SBD_GET_STATUS_EXTENDED      "AT+SBDSX\r"

/**
 * @def     AT_CMD_SBD_SET_TIMEOUT
 * @brief   Set the SBD session timeout.
 * 
 * Must be followed by "=<timeout>". Session timeout length is in seconds. 
 * Value 0 codes for infinite timeout. This specifies the maximum time allowed 
 * for an SBD session to complete. The timeout applies to any session commanded 
 * via the AT Command interface, i.e. by any of the following commands: 
 * +SBDI[X[A]] 
 * +SBDREG 
 * +SBDDET
 */
#define AT_CMD_SBD_SET_TIMEOUT              "AT+SBDST"

/**
 * @def     AT_CMD_SBD_CLEAR_MSG_SEQ_NB
 * @brief   This command will clear the mobile originated message sequence number (MOMSN) stored in the ISU.
 */
#define AT_CMD_SBD_CLEAR_MSG_SEQ_NB         "AT+SBDC\r"

/**
 * @def     AT_CMD_SBD_SET_DELIVERY_SHORT_CODE
 * @brief   Set the Delivery Short Code (DSC), which provides dynamic routing or control information for MO or MT messages.
 * 
 * Must be followed by "=<dsc>".
 * Set the Delivery Short Code (DSC), which provides dynamic routing or control information 
 * for MO or MT messages. This is an 8-bit value providing the ability to set individual fields. 
 * Value 0x80 (hexadecimal) sets the most significant bit. Value 0x01 sets the least significant bit. 
 * Flag values can be added together to achieve a combination of settings. Some fields are overridden 
 * during certain SBD sessions (e.g. an +SBDREG registration session sets flag 0x80).
 */
#define AT_CMD_SBD_SET_DELIVERY_SHORT_CODE  "AT+SBDDSC"

/**
 * @def     AT_CMD_SBD_SET_TERMINATED_ALERT
 * @brief   Enable or disable ring indications for SBD Ring Alerts.
 * 
 * "n" must be :
 * - 0 : Disable ring indication.
 * - 1 : Enable ring indication (default).
 */
#define AT_CMD_SBD_SET_TERMINATED_ALERT     "AT+SBDMTA=n\r"

/**
 * @def     AT_CMD_SBD_TRANSFER_MO_MT
 * @brief   This command will transfer the contents of the mobile originated buffer to the mobile terminated buffer.
 */
#define AT_CMD_SBD_TRANSFER_MO_MT           "AT+SBDTC\r"

/************************************/
/********* ADVANCED COMMANDS ********/
/************************************/

/**
 * @def     AT_CMD_SET_DTR
 * @brief   Set the ISU reaction to DTR (Data Terminal Ready) signal.
 * 
 * "n" must be :
 * - 0 : DTR is ignored in all modes.
 * - 1 : If DTR transitions from ON to OFF during in-call command mode,
 *       and DTR is restored ON within approximately 10 seconds, the call 
 *       will remain up. If DTR is not restored ON within approximately 
 *       10 seconds, the call will drop to on-hook command mode.
 *       If DTR transitions from ON to OFF during in-call data mode, the mode 
 *       will change to in- call command mode. If DTR is restored ON within 
 *       approximately 10 seconds, the call will remain up. If DTR is not restored 
 *       ON within approximately 10 seconds, the call will drop to on-hook
 *       command mode.
 * - 2 : If DTR transitions from ON to OFF during either in-call command mode or 
 *       in-call data mode, the call will drop to on-hook command mode (default).
 * - 3 : If DTR transitions from ON to OFF during either in-call command mode or 
 *       in-call data mode, the call will drop to on-hook command mode and the ISU 
 *       will reset to AT command profile 0.
 */
#define AT_CMD_SET_DTR                      "AT&Dn\r"

/**
 * @def     AT_CMD_SET_RTC
 * @brief   Set the real-time clock of the ISU.
 * 
 * Must be followed by "=[<time>]", where <time> is string type value; format is “yy/MM/dd,hh:mm:sszz”, 
 * and indicate year (two last digits), month, day, hour, minutes, seconds and time zone. 
 * There is no blank space between the two double quotes. Since time zone feature is not supported in Iridium, 
 * this particular field (+/-zz) is ignored if it is entered. The range of valid years is between 1970 and 2058.
 */
#define AT_CMD_SET_RTC                      "AT+CCLK"

/**
 * @def     AT_CMD_GET_MANUFACT_ID
 * @brief   Get manufacturer identification.
 */
#define AT_CMD_GET_MANUFACT_ID              "AT+CGMI\r"

/**
 * @def     AT_CMD_GET_MANUFACT_ID_ALIAS
 * @brief   Get manufacturer identification.
 */
#define AT_CMD_GET_MANUFACT_ID_ALIAS        "AT+GMI\r"

/**
 * @def     AT_CMD_GET_MODEL_ID
 * @brief   Get model identification.
 */
#define AT_CMD_GET_MODEL_ID                 "AT+CGMM\r"

/**
 * @def     AT_CMD_GET_MODEL_ID_ALIAS
 * @brief   Get model identification.
 */
#define AT_CMD_GET_MODEL_ID_ALIAS           "AT+GMM\r"

/**
 * @def     AT_CMD_GET_REVISION_NB
 * @brief   Get revision number.
 */
#define AT_CMD_GET_REVISION_NB              "AT+CGMR\r"

/**
 * @def     AT_CMD_GET_REVISION_NB_ALIAS
 * @brief   Get revision number.
 */
#define AT_CMD_GET_REVISION_NB_ALIAS        "AT+GMR\r"

/**
 * @def     AT_CMD_GET_SERIAL_NB
 * @brief   Get serial number.
 */
#define AT_CMD_GET_SERIAL_NB                "AT+CGSN\r"

/**
 * @def     AT_CMD_GET_SERIAL_NB_ALIAS
 * @brief   Get serial number.
 */
#define AT_CMD_GET_SERIAL_NB_ALIAS          "AT+GSN\r"

/**
 * @def     AT_CMD_SET_EVENT_REPORT
 * @brief   Set indicator event reporting.
 * 
 * Must be followed by "=[<mode>[,<sigind>[,<svcind>[,<antind>[,<sv_beam_coords_ind>]]]]]"
 * Look for the datasheet for more information.
 */
#define AT_CMD_SET_EVENT_REPORT             "AT+CIER"

/**
 * @def     AT_CMD_GET_RING_IND_STAT
 * @brief   Get ring indication status.
 */
#define AT_CMD_GET_RING_IND_STAT            "AT+CRIS\r"

/**
 * @def     AT_CMD_GET_RING_IND_STAT_TIMESTAMP
 * @brief   Get ring indication status with timestamp.
 */
#define AT_CMD_GET_RING_IND_STAT_TIMESTAMP  "AT+CRISX\r"

/**
 * @def     AT_CMD_GET_SIGNAL_QUALITY
 * @brief   Get signal quality.
 */
#define AT_CMD_GET_SIGNAL_QUALITY           "AT+CSQ\r"

/**
 * @def     AT_CMD_UNLOCK_SDB
 * @brief   Unlock the SBD functionality of the ISU after it has been locked by the Gateway.
 * 
 * Must be followed by "=<unlock key>", where <unlock key> is a string of 16 hexadecimal digits.
 */
#define AT_CMD_UNLOCK_SDB                   "AT+CULK"

/**
 * @def     AT_CMD_GET_SET_ENERGY_MON
 * @brief   Get or set energy monitor.
 * 
 * Must be followed by "=<n>" :
 * Preset the energy monitor accumulator to value n (typically, <n> would be specified as 0, 
 * to clear the accumulator). Where <n> is an estimate of the charge taken from the +5V supply 
 * to the modem, in microamp hours. This is represented internally as a 26-bit unsigned number, 
 * so in principle will rollover to zero after approx. 67Ah (in practice this is usually greater 
 * than battery life, if battery-powered).
 */
#define AT_CMD_GET_SET_ENERGY_MON           "AT+GEMON"

/**
 * @def     AT_CMD_SET_DATA_RATE
 * @brief   Get or set data rate.
 * 
 * Must be followed by "=<rate>[,<autobaud>]". Where <rate> is :
 * - 1 : 600 bps
 * - 2 : 1200 bps
 * - 3 : 2400 bps
 * - 4 : 4800 bps
 * - 5 : 9600 bps
 * - 6 : 19200 bps (default)
 * - 7 : 38400 bps
 * - 8 : 57600 bps
 * - 9 : 115200 bps
 * And <autobaud> takes the following values:
 * - 0 : disable autobaud
 * - 1 : enable autobaud
 */
#define AT_CMD_SET_DATA_RATE            "AT+IPR=n\r"

/**
 * @def     AT_CMD_REQUEST_GEOLOC
 * @brief   Request Geolocation.
 */
#define AT_CMD_REQUEST_GEOLOC               "AT-MSGEO\r"

/**
 * @def     AT_CMD_REQUEST_SYS_TIME
 * @brief   Request System Time.
 */
#define AT_CMD_REQUEST_SYS_TIME             "AT-MSSTM\r"

#endif /* IRIDIUM_CONSTANTS_H */

/** 
 * @} 
 */