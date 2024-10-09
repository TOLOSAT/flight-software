/**
 * @file    user_diskio.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT Disk IO functions
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

extern DSTATUS DiskInitialize(BYTE disk);
extern DSTATUS DiskStatus(BYTE disk);
extern DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
extern DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* USER_DISKIO_H */
