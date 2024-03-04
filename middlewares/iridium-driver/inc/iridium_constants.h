/**
 * @file    iridium_constants.h
 * @author  TO BE FILLED
 * @brief   Header file for Iridium Constants
 * @date    22/10/2023
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

/* Iridium Message Format */
#define AT_MSG_HEADER                       "AT"        /**< Headers for an AT message */
#define AT_REPEAT_LAST_COMMAND              "A/"        /**< Repeat the last command issued to the ISU unless the power was interrupted or the unit is reset. A/ is not followed by <CR>. */
#define AT_MSG_TRAILER                      "\n\r"      /**< Trailer for an AT message */

/* Iridium Command List */
#define AT_CMD_DISPLAY_REGS                 "%R"        /**< Display all the S registers in the system. */
#define AT_CMD_SET_DTR                      "&D"        /**< Set the ISU reaction to DTR signal. */
#define AT_CMD_FACTORY_RESET                "&F"        /**< Recall factory defaults. */
#define AT_CMD_SET_FLOW_CTRL                "&K"        /**< Select the flow control method between the ISU and DTE. */
#define AT_CMD_VIEW_CONF                    "&V"        /**< View the current active configuration and stored profiles. */
#define AT_CMD_WRITE_CONF                   "&W"        /**< Store the active profile in non-volatile memory. */
#define AT_CMD_SET_DEFAULT_PROF             "&Y"        /**< Designate Default Reset Profile. */
#define AT_CMD_FLUSH_EEPROM                 "*F"        /**< Flush all pending writes to Eeprom, shut down the radio, and prepare the Data Module to be powered down. */
#define AT_CMD_SET_RADIO_ACTIVITY           "*R"        /**< Set the radio activity. */
#define AT_CMD_SET_RTC                      "+CCLK"     /**< Set the real-time clock of the ISU. */
#define AT_CMD_GET_MANUFACT_ID              "+CGMI"     /**< Get manufacturer identification. */
#define AT_CMD_GET_MODEL_ID                 "+CGMM"     /**< Get model identification. */
#define AT_CMD_GET_REVISION_NB              "+CGMR"     /**< Get revision number. */
#define AT_CMD_GET_SERIAL_NB                "+CGSN"     /**< Get serial number. */
#define AT_CMD_SET_EVENT_REPORT             "+CIER"     /**< Set indicator event reporting. */
#define AT_CMD_GET_RING_IND_STAT            "+CRIS"     /**< Get ring indication status*/
#define AT_CMD_GET_SIGNAL_QUALITY           "+CSQ"      /**< Get signal quality */
#define AT_CMD_UNLOCK_SDB                   "+CULK"     /**< Unlock the SBD functionality of the ISU after it has been locked by the Gateway. */
#define AT_CMD_GET_SET_ENERGY_MON           "+GEMON"    /**< Get or set energy monitor. */
#define AT_CMD_GET_MANUFACT_ID_ALIAS        "+GMI"      /**< Get manufacturer identification. */
#define AT_CMD_GET_MODEL_ID_ALIAS           "+GMM"      /**< Get model identification. */
#define AT_CMD_GET_REVISION_NB_ALIAS        "+GMR"      /**< Get revision number. */
#define AT_CMD_GET_SERIAL_NB_ALIAS          "+GSN"      /**< Get serial number. */
#define AT_CMD_SET_GET_DATA_RATE            "+IPR"      /**< Get or set data rate. */
#define AT_CMD_SBD_SET_AUTO_REGIS_MODE      "+SBDAREG"  /**< Set the ISU’s Auto-registration SBD mode. */
#define AT_CMD_SBD_CLEAR_MSG_SEQ_NB         "+SBDC"     /**< This command will clear the mobile originated message sequence number (MOMSN) stored in the ISU. */
#define AT_CMD_SBD_CLEAR_MSG_BUFFER         "+SBDD"     /**< This command is used to clear the mobile originated buffer, mobile terminated buffer or both. */
#define AT_CMD_SBD_GATEWAY_DETACH           "+SBDDET"   /**< Initiates an SBD session to detach the ISU from the Gateway. */
#define AT_CMD_SBD_SET_DELIVERY_SHORT_CODE  "+SBDDSC"   /**< Set the Delivery Short Code (DSC), which provides dynamic routing or control information for MO or MT messages. */
#define AT_CMD_SBD_INIT_SESSION             "+SBDI"     /**< This command initiates an SBD session between the ISU and the ESS. */
#define AT_CMD_SBD_INIT_SESSION_EXTENDED    "+SBDIX"    /**< This command initiates an SBD session between the ISU and the GSS, setting the SBD Session Type. */
#define AT_CMD_SBD_SET_TERMINATED_ALERT     "+SBDMTA"   /**< Enable or disable ring indications for SBD Ring Alerts. */
#define AT_CMD_SBD_READ_BIN_DATA            "+SBDRB"    /**< This command is used to read binary data from ISU. */
#define AT_CMD_SBD_INIT_MANUAL_REGISTRATION "+SBDREG"   /**< Triggers an SBD session to perform a manual SBD registration. */
#define AT_CMD_SBD_READ_TEXT_DATA           "+SBDRT"    /**< Read a text message from the ISU. */
#define AT_CMD_SBD_GET_STATUS               "+SBDS"     /**< Get SBD status. */
#define AT_CMD_SBD_SET_TIMEOUT              "+SBDST"    /**< Set the SBD session timeout.*/
#define AT_CMD_SBD_GET_STATUS_EXTENDED      "+SBDSX"    /**< Get SBD extended status. */
#define AT_CMD_SBD_TRANSFER_MO_MT           "+SBDTC"    /**< This command will transfer the contents of the mobile originated buffer to the mobile terminated buffer. */
#define AT_CMD_SBD_WRITE_BIN_DATA           "+SBDWB"    /**< Write binary data to the ISU. */
#define AT_CMD_SBD_WRITE_TEXT_DATA          "+SBDWT"    /**< Write a text message from the ISU. */
#define AT_CMD_REQUEST_GEOLOC               "-MSGEO"    /**< Request Geolocation */
#define AT_CMD_REQUEST_SYS_TIME             "-MSSTM"    /**< Request System Time */
#define AT_CMD_ECHO                         "En"        /**< Echo command characters. */
#define AT_CMD_QUIET_MODE                   "Qn"        /**< Control ISU responses. */
#define AT_CMD_VERBOSE_MODE                 "Vn"        /**< Set the response format of the ISU, which may be either numeric or textual. */
#define AT_CMD_SOFT_RESET                   "Zn"        /**< Reset the ISU to a user-stored configuration. */

#endif /* IRIDIUM_CONSTANTS_H */

/** 
 * @} 
 */