/**
 * @file    memdrv_sd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card using SDMMC driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 * Adapted from STMicroelectronic example
 */

#ifndef MEMDRV_SD_H
#define MEMDRV_SD_H

/******************************* Include Files *******************************/

#include "boot_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t SD_Init(uint8_t disk);
extern returnCode_t SD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t SD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t SD_Ioctl(uint8_t disk, uint8_t cmd, void *data);
extern DSTATUS SD_GetStatus(uint8_t disk);

#endif /* MEMDRV_SD_H */