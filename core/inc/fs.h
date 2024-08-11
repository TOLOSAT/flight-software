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

#include "core_types.h"
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
 * @struct  fsFileDesc_t
 * @brief   Struct type of a file configuration
 */
typedef struct
{
    fsFileno_t ref;                   /**< @brief File numero as it is declared in FILE_DEVICE_ENUM */                            
    fsfileName_t *name;               /**< @brief File name */
    fsfileAccessMode_t access_mode;   /**< @brief File access mode */
    FIL *temp_file;                   /**< @brief Pointer to the temporary file */
} fsFileDesc_t;

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

extern coreStatus_t FsOpen(void);
extern coreStatus_t FsWrite(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
extern coreStatus_t FsRead(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size);
extern coreStatus_t FsGetFileSize(fsFileno_t fileno, fsSize_t *file_size);
extern coreStatus_t FsClose(void);

#endif /* FS_H */
