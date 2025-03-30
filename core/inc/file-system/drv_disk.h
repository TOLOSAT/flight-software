/**
 * @file    drv_disk.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT Disk IO functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef DRV_DISK_H
#define DRV_DISK_H

/******************************* Include Files *******************************/

#include "boot_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern DSTATUS DiskInitialize(BYTE disk);
extern DSTATUS DiskStatus(BYTE disk);
extern DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* DRV_DISK_H */
