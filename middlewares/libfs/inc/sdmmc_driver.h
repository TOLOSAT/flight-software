/**
 * @file    sdmmc_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card using SDMMC driver
 * @date    29/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

#ifndef SDMMC_DRIVER_H
#define SDMMC_DRIVER_H

/******************************* Include Files *******************************/

#include "fs_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

DSTATUS SD_GetStatus(uint8_t disk);
fsStatus_t SD_Init(uint8_t disk);
fsStatus_t SD_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
fsStatus_t SD_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
fsStatus_t SD_Ioctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* SDMMC_DRIVER_H */