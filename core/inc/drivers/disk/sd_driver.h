/**
 * @file    sd_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card disk using SDMMC driver
 * @date    29/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

#ifndef SD_DRIVER_H
#define SD_DRIVER_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS SD_DiskStatus(uint8_t disk);
extern halStatus_t SD_DiskInit(uint8_t disk);
extern halStatus_t SD_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern halStatus_t SD_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern halStatus_t SD_DiskIoctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* SD_DRIVER_H */