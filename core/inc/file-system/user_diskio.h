/**
 * @file    user_diskio.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT Disk IO functions
 * @date    17/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef USER_DISKIO_H
#define USER_DISKIO_H

/******************************* Include Files *******************************/

#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

DSTATUS DiskInitialize(BYTE disk);
DSTATUS DiskStatus(BYTE disk);
DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* USER_DISKIO_H */
