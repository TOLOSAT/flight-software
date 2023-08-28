/**
 * @file    tolosat_hal_fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_fs Driver FS
 * Functions that allows to drive File System
 * @{
 */

#ifndef TOLOSAT_HAL_FS_H
#define TOLOSAT_HAL_FS_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF               0x00u   /**< Disk0 reference */

/* Definitions for MMC/SDC command */
#define CMD_MSG_SIZE            6u      /**< Command Size*/
#define CMD0                    0x40u   /**< Command GO_IDLE_STATE */
#define CMD1                    0x41u   /**< Command SEND_OP_COND */
#define CMD8                    0x48u   /**< Command SEND_IF_COND */
#define CMD9                    0x49u   /**< Command SEND_CSD */
#define CMD10                   0x4au   /**< Command SEND_CID */
#define CMD12                   0x4cu   /**< Command STOP_TRANSMISSION */
#define CMD16                   0x50u   /**< Command SET_BLOCKLEN */
#define CMD17                   0x51u   /**< Command READ_SINGLE_BLOCK */
#define CMD18                   0x52u   /**< Command READ_MULTIPLE_BLOCK */
#define CMD23                   0x57u   /**< Command SET_BLOCK_COUNT */
#define CMD24                   0x58u   /**< Command WRITE_BLOCK */
#define CMD25                   0x59u   /**< Command WRITE_MULTIPLE_BLOCK */
#define CMD41                   0x69u   /**< Command SEND_OP_COND (ACMD) */
#define CMD55                   0x77u   /**< Command APP_CMD */
#define CMD58                   0x7au   /**< Command READ_OCR */

/* MMC/SDC Card types */
#define CT_NO_TYPE              0x00u   /**< Card has no type */
#define CT_MMC                  0x01u   /**< Card type MMC ver 3 */
#define CT_SD1                  0x02u   /**< Card type SD ver 1 */
#define CT_SD2                  0x04u   /**< Card type SD ver 2 */
#define CT_SDC                  0x06u   /**< Card type SD */
#define CT_BLOCK                0x08u   /**< Card type Block addressing */

/* SD Card constants */
#define SD_CNT_TIMEOUT          10000u      /**< SD Counter maximum value */
#define SD_WAKEUP_MSG_SIZE      10u         /**< Wakeup message size*/
#define SD_CS_PORT              GPIOA       /**< GPIO Port of SD card CS Pin */
#define SD_CS_PIN               GPIO_PIN_4  /**< GPIO Pin of SD card CS Pin */

/* SD card Status Flag */
#define SD_IDLE_FLAG            0x01u       /**< SD card IDLE flag position */
#define SD_ERASE_RST_FLAG       0x02u       /**< SD card ERASE RESET flag position */
#define SD_ILLEGAL_CMD_FLAG     0x04u       /**< SD card ILLEGAL COMMAND flag position */
#define SD_CRC_ERROR_FLAG       0x08u       /**< SD card CRC ERROR flag position */
#define SD_ERASE_ERROR_FLAG     0x10u       /**< SD card ERASE ERROR flag position */
#define SD_ADDR_ERROR_FLAG      0x20u       /**< SD card ADDR ERROR flag position */
#define SD_PARAM_ERROR_FLAG     0x40u       /**< SD card PARAM ERROR flag position */

/***************************** Types Definitions *****************************/

/** 
 * @enum    SDCardStatus_t
 * @brief   SD card status type enum
 */
typedef enum
{
    SD_CARD_OFF = 0u,   /**< @brief SD is OFF */
    SD_CARD_ON = 1u,    /**< @brief SD is ON */
} SDCardStatus_t;

/** 
 * @struct  FsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];          /**< @brief FS disk path */
    Diskio_drvTypeDef  driver;  /**< @brief FS driver */
    FATFS file_system;          /**< @brief FS */
} FsInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t FsOpen(FsInst_t *fs_inst);

#endif /* TOLOSAT_HAL_FS_H */

/** 
 * @} 
 * @} 
 */