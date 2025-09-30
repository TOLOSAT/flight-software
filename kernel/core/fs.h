/**
 * @file    fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT File System functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
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

#include <ff.h>
#include <ffconf.h>
#include <ff_gen_drv.h>

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     FILE_CONF(file_no)
 * @brief   Get file conf from g_files_conf_table
 */
#define FILE_CONF(file_no) (g_files_conf_table[(file_no) - 1u])

/**
 * @def     FILE_DESC(file_no)
 * @brief   Get file conf from g_files_desc_table
 */
#define FILE_DESC(file_no) (g_files_desc_table[(file_no) - 1u])

/**
 * @def     IS_A_VALID_FILE(file_no)
 * @brief   Indicates if the file_no is valid
 */
#define IS_A_VALID_FILE(file_no) \
    (((file_no) != (fileNo_t)NO_FILE) && ((file_no) < (fileNo_t)CONFIG_MAX_NB_FILES) && (FILE_DESC(file_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/**
 * @struct  fsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];        /**< @brief FS disk path */
    Diskio_drvTypeDef driver; /**< @brief FS driver */
    FATFS file_system;        /**< @brief FS */
} fsInst_t;

/**
 * @struct  fsFileDesc_t
 * @brief   Struct type of a file descriptor
 */
typedef struct
{
    descStatus_t status; /**< @brief Indicates if the descriptor is free or used */
    FIL temp_file;       /**< @brief Pointer to the temporary file */
} fsFileDesc_t;

/*************************** Variables Declarations **************************/

extern const fsFileConf_t g_files_conf_table[CONFIG_MAX_NB_FILES];
extern fsFileDesc_t g_files_desc_table[CONFIG_MAX_NB_FILES];

/*************************** Functions Declarations **************************/

extern void InitFs(void);
extern void CreateFsMutexes(void);
extern returnCode_t FsWrite(fileNo_t file, data_t data, length_t length);
extern returnCode_t FsRead(fileNo_t file, data_t data, length_t length);
extern returnCode_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t DeinitFs(void);

#endif /* FS_H */

/**
 * @}
 * @}
 * @}
 */