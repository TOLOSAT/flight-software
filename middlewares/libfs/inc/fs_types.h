/**
 * @file    fs_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System types
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef FS_TYPES_H
#define FS_TYPES_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

#define IN_FS_TEXT_SECTION     __attribute__((section(".text_fs")))   /**< FS functions goes in the .text_fs */
#define IN_FS_DATA_SECTION     __attribute__((section(".data_fs")))   /**< FS data goes in the .data_fs */

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
    FS_SUCCESSFUL = 0u,     /**< Function succeed */
    FS_ERROR = 1u,          /**< Function failed */
    FS_INVALID_PARAM = 2u,  /**< Function parameter is not valid */
    FS_UNAVAILABLE = 3u,    /**< Function is not available */
    FS_TIMEOUT = 4u,        /**< Device timed out */
    FS_BUSY = 5u,           /**< Device busy */
} fsStatus_t;

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
    FIL *temp_file;                   /**< @brief Pointer to the temporary file */
} fsFileConf_t;

#endif /* FS_TYPES_H */
