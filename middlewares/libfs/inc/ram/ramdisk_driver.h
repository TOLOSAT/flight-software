/**
 * @file    ramdisk_driver.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RAM driver
 * @date    21/07/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

#ifndef RAMDISK_DRIVER_H
#define RAMDISK_DRIVER_H

/******************************* Include Files *******************************/

#include "fs_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS RAMDisk_GetStatus(uint8_t disk);
extern fsStatus_t RAMDisk_Init(uint8_t disk);
extern fsStatus_t RAMDisk_ReadBlocks(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern fsStatus_t RAMDisk_WriteBlocks(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern fsStatus_t RAMDisk_Ioctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* RAMDISK_DRIVER_H */