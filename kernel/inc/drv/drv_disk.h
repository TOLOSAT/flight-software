/**
 * @file    drv_disk.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for disk driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_disk Disk Driver
 * @brief Abstraction layer for managing disk operations for the file-system.
 * @{
 */

#ifndef DRV_DISK_H
#define DRV_DISK_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS DiskInitialize(BYTE disk);
extern DSTATUS DiskStatus(BYTE disk);
extern DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* DRV_DISK_H */

/** 
 * @}
 * @}
 * @}
 */