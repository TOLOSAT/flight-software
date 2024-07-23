/**
 * @file    fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "fs.h"
#include "conf/fs_conf.h"
#include "user_diskio.h"
#include "mutex.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static FRESULT FsBuildFileSystem(void);
static FRESULT CreateParentDirectories(const char *path);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              FsOpen(fsInst_t *fs_inst)
 * @brief           Function that initialise a FS
 * @param[in,out]   fs_inst Instance that contains FS parameters and driver
 * @retval          #FS_INVALID_PARAM if fs_inst is null pointer
 * @retval          #FS_ERROR if cannot create FS
 * @retval          #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION FsOpen(fsInst_t *fs_inst)
{
#if defined(FS_MODE_NONE)
    // Unused variables
    (void)(fs_inst);

    // Always return successfull
    return FS_SUCCESSFUL;
#else
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        // Link driver function
        fs_inst->driver.disk_initialize = DiskInitialize;
        fs_inst->driver.disk_status = DiskStatus;
        fs_inst->driver.disk_read = DiskRead;
        fs_inst->driver.disk_write = DiskWrite;
        fs_inst->driver.disk_ioctl = DiskIoctl;

        // We link driver functions to FATFS
        uint8_t test_fs = FATFS_LinkDriver(&fs_inst->driver, fs_inst->disk_path);
        if (test_fs != 0u)
        {
            return_value = FS_ERROR;
        }
        else
        {
            // Then we mount the disk
            test_fs = f_mount(&fs_inst->file_system, "/", 1);
            if (test_fs == FR_NO_FILESYSTEM)
            {
                test_fs = FsBuildFileSystem();
            }

            // Check if mount went right
            if (test_fs == FR_OK)
            {
                // Now open all files
                fsFileno_t fileno = 0u;
                while ((fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES) && (test_fs == FR_OK))
                {
                    test_fs = f_open(g_files_conf[SD0][fileno].temp_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
                    fileno++;
                }

                // Check if no error occured
                if (test_fs != FR_OK)
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsWrite(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
 * @brief       Function that write into a file of the fS
 * @param[in]   fileno File reference numero
 * @param[in]   offset Offset from where data will be written
 * @param[in]   data Pointer to data which will be written
 * @param[in]   size Size of data
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_UNAVAILABLE if FS is already use by another thread
 * @retval      #FS_ERROR if fatfs function has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION FsWrite(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
#if defined(FS_MODE_NONE)
    // Unused variables
    (void)(fileno);
    (void)(offset);
    (void)(data);
    (void)(size);

    // Always return successfull
    return FS_SUCCESSFUL;
#else
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (size != 0u) && (fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES))
    {
        // Places the write pointer in the right place
        test_fs = f_lseek(g_files_conf[SD0][fileno].temp_file, offset);
        if (test_fs == FR_OK)
        {
            // Copy data onto file
            uint32_t bytes_written = 0u;
            test_fs = f_write(g_files_conf[SD0][fileno].temp_file, data, size, (UINT *)&bytes_written);
            if ((test_fs == FR_OK) && (bytes_written == size))
            {
                // Sync file
                test_fs = f_sync(g_files_conf[SD0][fileno].temp_file);
                if (test_fs != FR_OK)
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsRead(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
 * @brief       Function that read from a file of the fS
 * @param[in]   fileno File reference numero
 * @param[in]   offset Offset from where data will be read
 * @param[out]  data Pointer to data which will be read
 * @param[in]   size Size of data
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_UNAVAILABLE if FS is already use by another thread
 * @retval      #FS_ERROR if fatfs function has encountered an error
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION FsRead(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
#if defined(FS_MODE_NONE)
    // Unused variables
    (void)(fileno);
    (void)(offset);
    (void)(data);
    (void)(size);

    // Always return successfull
    return FS_SUCCESSFUL;
#else
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (size != 0u) && (fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES))
    {
        // Places the write pointer in the right place
        test_fs = f_lseek(g_files_conf[SD0][fileno].temp_file, offset);
        if (test_fs == FR_OK)
        {
            // Copy data onto file
            uint32_t bytes_read = 0u;
            test_fs = f_read(g_files_conf[SD0][fileno].temp_file, data, size, (UINT *)&bytes_read);
            if ((test_fs != FR_OK) || (bytes_read != size))
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsGetFileSize(fsFileno_t fileno, fsSize_t *file_size)
 * @brief       Functions that gets file size
 * @param[in]   fileno
 * @param[out]  file_size
 * @retval      #FS_INVALID_PARAM if a pointer is null
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION FsGetFileSize(fsFileno_t fileno, fsSize_t *file_size)
{
#if defined(FS_MODE_NONE)
    // Unused variables
    (void)(fileno);
    (void)(file_size);

    // Always return successfull
    return FS_SUCCESSFUL;
#else
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (file_size != NULL)
    {
        // Get size
        *file_size = f_size(g_files_conf[SD0][fileno].temp_file);
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsClose(fsInst_t *fs_inst)
 * @brief       Function that desinit the disk (and FS) connection and puts defaults parameters
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t IN_FS_TEXT_SECTION FsClose(fsInst_t *fs_inst)
{
#if defined(FS_MODE_NONE)
    // Unused variables
    (void)(fs_inst);

    // Always return successfull
    return FS_SUCCESSFUL;
#else
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        // First we close every file
        uint8_t test_fs = FR_OK;
        fsFileno_t fileno = 0u;
        while ((fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES) && (test_fs == FR_OK))
        {
            test_fs = f_close(g_files_conf[SD0][fileno].temp_file);
            fileno++;
        }

        // Check if no error occured
        if (test_fs == FR_OK)
        {
            // Link driver function
            fs_inst->driver.disk_initialize = NULL;
            fs_inst->driver.disk_status = NULL;
            fs_inst->driver.disk_read = NULL;
            fs_inst->driver.disk_write = NULL;
            fs_inst->driver.disk_ioctl = NULL;

            // We unlink driver functions to FATFS
            test_fs = FATFS_UnLinkDriverEx(fs_inst->disk_path, 0u);
            if (test_fs != 0u)
            {
                return_value = FS_ERROR;
            }
        }
        else
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsBuildFileSystem(void)
 * @brief       Function that rebuild the file system if not present on the drive
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @return      FRESULT
 *
 * @warning This function will recreate a file system so it will potentially erase data if any
 */
static FRESULT IN_FS_TEXT_SECTION FsBuildFileSystem(void)
{
#if defined(FS_MODE_NONE)
    // Always return successfull
    return FR_OK;
#else
    // Variable initialisation
    FRESULT return_value = FR_OK;
    uint8_t work[FF_MAX_SS] = {0};
    fsFileno_t fileno = 0u;

    // Function Core
    return_value = f_mkfs("/", 0, work, FF_MAX_SS);

    // Now create parent directories for every file
    while ((return_value == FR_OK) && (fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES))
    {
        return_value = CreateParentDirectories(g_files_conf[SD0][fileno].name);
        fileno++;
    }

    return return_value;
#endif
}

/**
 * @fn          CreateParentDirectories(const char *path)
 * @brief       Function that create a directory for every dir in a file path
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @return      FRESULT
 */
static FRESULT IN_FS_TEXT_SECTION CreateParentDirectories(const char *path)
{
    // Variable initialisation
    FRESULT res = FR_OK;
    char tmp_path[FF_MAX_LFN];
    char *separator;

    // First copy the path in the buffer
    strcpy(tmp_path, path);

    // Browse the path and create each missing directory
    separator = strchr(tmp_path, '/');
    while ((separator != NULL) && ((res == FR_OK) || (res == FR_EXIST)))
    {
        // Put 0 as the next separator by default
        *separator = '\0';

        // Create dir
        res = f_mkdir(tmp_path);

        // If dir was successfully created look a the next separator
        if ((res == FR_OK) || (res == FR_EXIST))
        {
            *separator = '/';
            separator = strchr(separator + 1, '/');
        }
    }

    // Just if FR_EXIST it means the dir already 
    // exist so we return FR_OK for compatibility
    if (res == FR_EXIST)
    {
        res = FR_OK;
    }

    return res;
}