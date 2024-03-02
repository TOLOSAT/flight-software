/**
 * @file    spisd_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card SPI driver 
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef SPISD_DRIVER_H
#define SPISD_DRIVER_H

/******************************* Include Files *******************************/

#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

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

DSTATUS SpiSD_GetStatus(uint8_t disk);
fsStatus_t SpiSD_Init(uint8_t disk);
fsStatus_t SpiSD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
fsStatus_t SpiSD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
fsStatus_t SpiSD_Ioctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* SPISD_DRIVER_H */
