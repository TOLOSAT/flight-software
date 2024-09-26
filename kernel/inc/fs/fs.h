/**
 * @file    fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup fs File System
 * @{
 * @defgroup fs_handling File System Handling
 * @brief File-system handling interface.
 * @{
 */

#ifndef FS_H
#define FS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "conf/fs_conf.h"
#include "ff_gen_drv.h"
#include "core/mutex.h"

/***************************** Macros Definitions ****************************/

#define FS_IOCTL_GET_SIZE           0u  /**< Get file size */
#define FS_IOCTL_SEEK               1u  /**< Moves read/write pointer in the file */
#define FS_IOCTL_SYNC               2u  /**< Synchronise file on the disk */
#define FS_IOCTL_TRANSFER_DATA      3u  /**< Transfer data from the current file to another one */

/***************************** Types Definitions *****************************/

/** @brief FS file Name type */
typedef const char fsfileName_t;

/** @brief FS file access mode type */
typedef uint8_t fsfileAccessMode_t;

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
 * @struct  fsFileConf_t
 * @brief   Struct type of a file configuration
 */
typedef struct
{                          
    fsfileName_t *name;             /**< @brief File name */
    fsfileAccessMode_t access_mode; /**< @brief File access mode */
    fsAutoSyncStatus_t auto_sync;   /**< @brief File automatic synchronisation setting */
    mutexQueue_t *p_mutex_queue;    /**< @brief Pointer to the file mutex queue */
} fsFileConf_t;

/** 
 * @struct  fsFileDesc_t
 * @brief   Struct type of a file descriptor
 */
typedef struct
{
    FIL *temp_file;         /**< @brief Pointer to the temporary file */
    mutexHandle_t mutex;    /**< @brief File mutex */
} fsFileDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_file_conf_table
 * @brief   Configuration table where all file configurations are stored
 */
extern fsFileConf_t g_file_conf_table[NB_FILES];

/**
 * @var     g_file_desc_table
 * @brief   Descriptor table where all file descriptors are stored
 */
extern fsFileDesc_t g_file_desc_table[NB_FILES];

/*************************** Functions Declarations **************************/

extern kernelStatus_t InitFs(void);
extern kernelStatus_t FsWrite(fileNo_t file, data_t data, length_t length);
extern kernelStatus_t FsRead(fileNo_t file, data_t data, length_t length);
extern kernelStatus_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size);
extern kernelStatus_t FsLock(fileNo_t file);
extern kernelStatus_t FsUnlock(fileNo_t file);
extern kernelStatus_t DeinitFs(void);

#endif /* FS_H */

/** 
 * @}
 * @}
 * @}
 */