/**
 * @file    sd_spi_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card SPI driver 
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef SD_SPI_DRIVER_H
#define SD_SPI_DRIVER_H

/******************************* Include Files *******************************/

#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/* Definitions for MMC/SDC command */
#define CMD_MSG_SIZE                6u          /**< Command Size */
#define CMD_MSG_ANSWER_SIZE         4u          /**< Command Answer Size */
#define CMD0                        0x40u       /**< Command GO_IDLE_STATE */
#define CMD1                        0x41u       /**< Command SEND_OP_COND */
#define CMD8                        0x48u       /**< Command SEND_IF_COND */
#define CMD9                        0x49u       /**< Command SEND_CSD */
#define CMD10                       0x4au       /**< Command SEND_CID */
#define CMD12                       0x4cu       /**< Command STOP_TRANSMISSION */
#define CMD16                       0x50u       /**< Command SET_BLOCKLEN */
#define CMD17                       0x51u       /**< Command READ_SINGLE_BLOCK */
#define CMD18                       0x52u       /**< Command READ_MULTIPLE_BLOCK */
#define CMD23                       0x57u       /**< Command SET_BLOCK_COUNT */
#define CMD24                       0x58u       /**< Command WRITE_BLOCK */
#define CMD25                       0x59u       /**< Command WRITE_MULTIPLE_BLOCK */
#define CMD41                       0x69u       /**< Command SEND_OP_COND (ACMD) */
#define CMD55                       0x77u       /**< Command APP_CMD */
#define CMD58                       0x7au       /**< Command READ_OCR */
#define NULL_COMMAND_ARG            0x00000000u /**< Command argument filled with 0 */

/* Definition for MMC/SDC tokens */
#define SD_START_BLOCK_TOKEN        0xfeu        /**< Token notifying start of a 512 bits block */
#define SD_START_MULT_BLOCK_TOKEN   0xfcu        /**< Token notifying start of multiple 512 bits blocks */
#define SD_STOP_TOKEN               0xfdu        /**< Token stoping 512 bits block transaction */

/* Definition for MMC/SDC transmit data response */
#define SD_DATA_RESPONSE_MASK       0x1fu        /**< SPI data transmit data response mask */
#define SD_DATA_ACCEPTED            0x05u        /**< SPI data transmit data accepted */
#define SD_DATA_CRC_ERROR           0x0bu        /**< SPI data transmit data crc error */
#define SD_DATA_WRITE_ERROR         0x0du        /**< SPI data transmit data write */

/* SD Card constants */
#define SD_CNT_TIMEOUT              10000u      /**< SD Counter maximum value */
#define SD_INITIALIZATION_TRIALS    200u        /**< How many time initialisation process will be executed until it succeeds */
#define SD_WAKEUP_MSG_SIZE          10u         /**< Wakeup message size*/
#define SD_BLOCK_SIZE               512u        /**< Card Block Size */
#define SD_INITIALIZATION_CONF      0x40000000u /**< SD card initialization configuration */
#define SD_CARD_INTERFACE_COND      0x000001aau /**< SD Card interface condition register (voltage setting and others) */
#define SD_CCS_BITMASK              0x40u       /**< Bitmask to access to CCS bit (Card Capacity status) if 1 then SD card is HC or XC */
#define SD_CS_PORT                  GPIOA       /**< GPIO Port of SD card CS Pin */
#define SD_CS_PIN                   GPIO_PIN_4  /**< GPIO Pin of SD card CS Pin */

/* SD card Status Flag */
#define SD_IDLE_FLAG                0x01u       /**< SD card IDLE flag position */
#define SD_ERASE_RST_FLAG           0x02u       /**< SD card ERASE RESET flag position */
#define SD_ILLEGAL_CMD_FLAG         0x04u       /**< SD card ILLEGAL COMMAND flag position */
#define SD_CRC_ERROR_FLAG           0x08u       /**< SD card CRC ERROR flag position */
#define SD_ERASE_ERROR_FLAG         0x10u       /**< SD card ERASE ERROR flag position */
#define SD_ADDR_ERROR_FLAG          0x20u       /**< SD card ADDR ERROR flag position */
#define SD_PARAM_ERROR_FLAG         0x40u       /**< SD card PARAM ERROR flag position */

/***************************** Types Definitions *****************************/

/** 
 * @enum    SDCardType_t
 * @brief   SD card type type enum
 */
typedef enum
{
    NOT_SDCARD = 0u,    /**< @brief Not an SD Card */
    SDCARD_V1 = 1u,     /**< @brief Is a SD card v1 */
    SDCARD_V2 = 2u,     /**< @brief Is a SD card v2 */
    SDCARD_V2HC = 3u,   /**< @brief Is a SD card v2 High Capacity */
} SDCardType_t;

/** 
 * @enum    SDCardStatus_t
 * @brief   SD card status type enum
 */
typedef enum
{
    SD_CARD_OFF = 0u,   /**< @brief SD is OFF */
    SD_CARD_ON = 1u,    /**< @brief SD is ON */
} SDCardStatus_t;

/*************************** Variables Declarations **************************/

extern SDCardStatus_t g_sd_card_status;
extern SDCardType_t g_sd_card_type;

/*************************** Functions Declarations **************************/

fsStatus_t SD_Select(void);
fsStatus_t SD_Unselect(void);
fsStatus_t SD_WaitUntilReady(void);
fsStatus_t SD_SwitchOn(void);
fsStatus_t SD_SwitchOff(void);
fsStatus_t SD_RxDataBlock(uint8_t *buff, uint32_t len);
fsStatus_t SD_TxDataBlock(const uint8_t *buff, uint32_t len, uint8_t token);
fsStatus_t SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t *answer, uint32_t answer_size);

#endif /* SD_SPI_DRIVER_H */
