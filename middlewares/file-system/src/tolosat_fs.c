/**
 * @file    tolosat_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tolosat_fs.h"
#include "conf/fs_conf.h"
#include "diskio.h"
#include "mutex.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

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
fsStatus_t FsOpen(fsInst_t *fs_inst)
{
#if defined(FS_NONE)
    // Unuse variables
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
            if (test_fs == FR_OK)
            {
                // Now open all files
                fsFileno_t fileno = 0u;
                while ((fileno < MAX_NB_FILES_PER_DEVICES) && (test_fs == FR_OK))
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
fsStatus_t FsWrite(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
#if defined(FS_NONE)
    // Unuse variables
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
fsStatus_t FsRead(fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
#if defined(FS_NONE)
    // Unuse variables
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
fsStatus_t FsGetFileSize(fsFileno_t fileno, fsSize_t *file_size)
{
#if defined(FS_NONE)
    // Unuse variables
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
fsStatus_t FsClose(fsInst_t *fs_inst)
{
#if defined(FS_NONE)
    // Unuse variables
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
        while ((fileno < MAX_NB_FILES_PER_DEVICES) && (test_fs == FR_OK))
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