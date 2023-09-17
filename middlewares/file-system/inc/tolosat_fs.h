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

fsStatus_t FsOpen(fsInst_t *fs_inst);
fsStatus_t FsWrite(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
fsStatus_t FsRead(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
fsStatus_t FsIoCtl(fsInst_t *fs_inst, fsIoCtlCmd_t io_cmd);
fsStatus_t FsClose(fsInst_t *fs_inst);

#endif /* TOLOSAT_FS_H */
