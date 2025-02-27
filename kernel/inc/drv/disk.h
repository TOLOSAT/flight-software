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

#ifndef DISK_H
#define DISK_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF 0x00u /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/** @brief Disk status type redefinition */
typedef DSTATUS diskStatus_t;

/** @brief Disk result type redefinition */
typedef DRESULT diskResult_t;

/** @brief Disk byte type redefinition */
typedef BYTE diskByte_t;

/** @brief Disk word type redefinition */
typedef DWORD diskWord_t;

/** @brief Disk unsigned int type redefinition */
typedef UINT diskUint_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern diskStatus_t DiskInitialize(diskByte_t disk);
extern diskStatus_t DiskStatus(diskByte_t disk);
extern diskResult_t DiskRead(diskByte_t disk, diskByte_t *buff, diskWord_t sector, diskUint_t count);
extern diskResult_t DiskWrite(diskByte_t disk, const diskByte_t *buff, diskWord_t sector, diskUint_t count);
extern diskResult_t DiskIoctl(diskByte_t disk, diskByte_t cmd, void *buff);

#endif /* DRV_DISK_H */

/**
 * @}
 * @}
 * @}
 */