/**
 * @file    diskdrv_nand.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for NAND disk using FMC NAND driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup diskdrv_nand NAND Disk Driver
 * @brief Abstraction layer for controlling an NAND card disk.
 * @{
 */

#ifndef DISKDRV_NAND_H
#define DISKDRV_NAND_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern diskStatus_t NAND_DiskStatus(uint8_t disk);
extern diskStatus_t NAND_DiskInit(uint8_t disk);
extern returnCode_t NAND_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t NAND_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t NAND_DiskIoctl(uint8_t disk, uint8_t cmd, void *data);

#endif /* DISKDRV_NAND_H */

/**
 * @}
 * @}
 * @}
 */