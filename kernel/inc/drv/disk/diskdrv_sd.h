/**
 * @file    diskdrv_sd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card disk using SDMMC driver
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup diskdrv_sd SD Disk Driver
 * @brief Abstraction layer for controlling an SD card disk.
 * @{
 */

#ifndef DISKDRV_SD_H
#define DISKDRV_SD_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS SD_DiskStatus(uint8_t disk);
extern returnCode_t SD_DiskInit(uint8_t disk);
extern returnCode_t SD_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t SD_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t SD_DiskIoctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* DISKDRV_SD_H */

/** 
 * @}
 * @}
 * @}
 */