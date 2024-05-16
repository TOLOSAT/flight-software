/**
 * @file    fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef FS_H
#define FS_H

/******************************* Include Files *******************************/

#include "fs_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    fsIoCtlAction_t
 * @brief   FS IOCTL action type enum
 */
typedef enum
{
    FILE_SIZE = 0u,   /**< Get File Size */
} fsIoCtlAction_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern fsStatus_t FsOpen(fsInst_t *fs_inst);
extern fsStatus_t FsWrite(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
extern fsStatus_t FsRead(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
extern fsStatus_t FsGetFileSize(fsFileno_t fileno, fsSize_t *file_size);
extern fsStatus_t FsClose(fsInst_t *fs_inst);

#endif /* FS_H */
