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

#define FS_IOCTL_GET_SIZE           0u  /**< Get file size */
#define FS_IOCTL_SYNC               1u  /**< Synchronise file on the disk */
#define FS_IOCTL_DISABLE_AUTO_SYNC  2u  /**< Disable file automatic synchronisation */
#define FS_IOCTL_ENABLE_AUTO_SYNC   3u  /**< Enable file automatic synchronisation */
#define FS_IOCTL_TRANSFER_DATA      4u  /**< Transfer data from the current file to another one */

/***************************** Types Definitions *****************************/

/** @brief FS file numero type definition */
typedef uint32_t fileNo_t;

/** @brief FS file Name type */
typedef const char fsfileName_t;

/** @brief FS file access mode type */
typedef uint8_t fsfileAccessMode_t;

/** @brief FS data type definition */
typedef uint8_t fsData_t;

/** @brief Size type definition */
typedef uint32_t fsSize_t;

/** 
 * @enum    fsAutoSyncStatus_t
 * @brief   FS file automatic synchronisation type enum
 */
typedef enum
{
    FS_AUTO_SYNC_DISABLE = 0u,   /**< File is not automatically synchronised */
    FS_AUTO_SYNC_ENABLE = 1u,    /**< File is automatically synchronised */
} fsAutoSyncStatus_t;

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
    fileNo_t file;                /**< @brief File numero as it is declared in FILE_DEVICE_ENUM */                            
    fsfileName_t *name;             /**< @brief File name */
    fsfileAccessMode_t access_mode; /**< @brief File access mode */
    fsAutoSyncStatus_t auto_sync;   /**< @brief File automatic synchronisation setting */
    FIL *temp_file;                 /**< @brief Pointer to the temporary file */
} fsFileDesc_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern coreStatus_t InitFs(void);
extern coreStatus_t FsWrite(fileNo_t file, fsSize_t offset, fsData_t *data, fsSize_t size);
extern coreStatus_t FsRead(fileNo_t file, fsSize_t offset, fsData_t *data, fsSize_t size);
extern coreStatus_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size);
extern coreStatus_t DeinitFs(void);

#endif /* FS_H */
