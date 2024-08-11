/**
 * @file    generic_hal_disk.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for disk driver functions
 * @date    17/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef GENERIC_HAL_DISK_H
#define GENERIC_HAL_DISK_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_DISK_TEXT_SECTION     __attribute__((section(".text_disk")))   /**< Disk functions goes in the .text_disk */
#define IN_DISK_DATA_SECTION     __attribute__((section(".data_disk")))   /**< Disk data goes in the .data_disk */

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS DiskInitialize(BYTE disk);
extern DSTATUS DiskStatus(BYTE disk);
extern DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* GENERIC_HAL_DISK_H */
