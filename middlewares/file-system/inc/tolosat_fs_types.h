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

/** @brief FS file numero type definition */
typedef uint32_t fsFileno_t;

/** @brief FS file Name type */
typedef const char fsfileName_t;

/** @brief FS file access mode type */
typedef uint8_t fsfileAccessMode_t;

/** @brief FS data type definition */
typedef uint8_t fsData_t;

/** @brief Size type definition */
typedef uint32_t fsSize_t;

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
 * @struct  fsIoCtlCmd_t
 * @brief   Generic struct type for IO control command
 */
typedef struct  {
    uint32_t cmd;       /**< @brief IO control command reference */
    uint32_t data_size; /**< @brief IO control data size (can be null) */
    void *data;         /**< @brief IO control data (inout) pointer (can be null) */
} fsIoCtlCmd_t;

/** 
 * @struct  fsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];          /**< @brief FS disk path */
    Diskio_drvTypeDef  driver;  /**< @brief FS driver */
    FATFS file_system;          /**< @brief FS */
} fsInst_t;

/** 
 * @struct  fsFileConf_t
 * @brief   Struct type of a file configuration
 */
typedef struct
{
    fsFileno_t ref;                   /**< @brief File numero as it is declared in FILE_DEVICE_ENUM */                            
    fsfileName_t *name;               /**< @brief File name */
    fsfileAccessMode_t access_mode;   /**< @brief File access mode */
} fsFileConf_t;

#endif /* TOLOSAT_FS_TYPES_H */
