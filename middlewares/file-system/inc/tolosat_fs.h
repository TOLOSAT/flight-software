/**
 * @file    tolosat_fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_FS_H
#define TOLOSAT_FS_H

/******************************* Include Files *******************************/

#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

fsStatus_t FsOpen(FsInst_t *fs_inst);
DSTATUS DiskInitialize(BYTE disk);
DSTATUS DiskStatus(BYTE disk);
DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);

#endif /* TOLOSAT_FS_H */
