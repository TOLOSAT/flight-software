/**
 * @file    tolosat_fs_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System types
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_FS_TYPES_H
#define TOLOSAT_FS_TYPES_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    fsStatus_t
 * @brief   TOLOSAT file system functions specific returns 
 */
typedef enum
{
    FS_SUCCESSFUL = 0u,    /**< Function succeed */
    FS_ERROR = 1u,         /**< Function failed */
    FS_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    FS_TIMEOUT = 3u,       /**< Device timed out */
} fsStatus_t;

/** 
 * @struct  FsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];          /**< @brief FS disk path */
    Diskio_drvTypeDef  driver;  /**< @brief FS driver */
    FATFS file_system;          /**< @brief FS */
} FsInst_t;

#endif /* TOLOSAT_FS_TYPES_H */
