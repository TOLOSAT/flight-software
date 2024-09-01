/**
 * @file    diskdrv_ram.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RAM disk driver
 * @date    21/07/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 * Adapted from STMicroelectronic example
 */

#ifndef DISKDRV_RAM_H
#define DISKDRV_RAM_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS RAM_DiskStatus(uint8_t disk);
extern coreStatus_t RAM_DiskInit(uint8_t disk);
extern coreStatus_t RAM_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern coreStatus_t RAM_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern coreStatus_t RAM_DiskIoctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* DISKDRV_RAM_H */