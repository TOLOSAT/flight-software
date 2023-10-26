/**
 * @file    tolosat_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include <FreeRTOS.h>
#include <semphr.h>

#include "tolosat_fs.h"
#include "conf/fs_conf.h"
#include "diskio.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_fs_buffer_file
 * @brief   Buffer file used when FsWrite, FsRead, or FsIoCtl are used
 */
static FIL g_fs_buffer_file = {0};

/**
 * @var     g_fs_mutex
 * @brief   Mutex used when FsWrite, FsRead, or FsIoCtl are used
 */
static SemaphoreHandle_t g_fs_mutex = NULL;

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
        uint8_t test_hal = FATFS_LinkDriver(&fs_inst->driver, fs_inst->disk_path);
        if (test_hal != 0u)
        {
            return_value = FS_ERROR;
        }
        else
        {
            // Then we mount the disk
            test_hal = f_mount(&fs_inst->file_system, "/", 1);
            if (test_hal == 0u)
            {
                // Create mutex for FS
                g_fs_mutex = xSemaphoreCreateMutex();
                if (g_fs_mutex == NULL)
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
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    BaseType_t mutex_status;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (size != 0u) && (fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES))
    {
        // First Acquire Mutex
        mutex_status = xSemaphoreTake(g_fs_mutex, 0u);
        if (mutex_status == pdTRUE)
        {
            // Open requested file
            test_fs = f_open(&g_fs_buffer_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
            if (test_fs == FR_OK)
            {
                // Places the write pointer in the right place
                test_fs = f_lseek(&g_fs_buffer_file, offset);
                if (test_fs == FR_OK)
                {
                    // Copy data onto file
                    uint32_t bytes_written = 0u;
                    test_fs = f_write(&g_fs_buffer_file, data, size, (UINT *)&bytes_written);
                    if ((test_fs == FR_OK) && (bytes_written == size))
                    {
                        // Close file
                        test_fs = f_close(&g_fs_buffer_file);
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
                return_value = FS_ERROR;
            }

            // Release Mutex anyway
            (void)xSemaphoreGive(g_fs_mutex);
        }
        else
        {
            return_value = FS_UNAVAILABLE;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
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
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    BaseType_t mutex_status;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (size != 0u) && (fileno < (fsFileno_t)MAX_NB_FILES_PER_DEVICES))
    {
        // First Acquire Mutex
        mutex_status = xSemaphoreTake(g_fs_mutex, 0u);
        if (mutex_status == pdTRUE)
        {
            // Open requested file
            test_fs = f_open(&g_fs_buffer_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
            if (test_fs == FR_OK)
            {
                // Places the write pointer in the right place
                test_fs = f_lseek(&g_fs_buffer_file, offset);
                if (test_fs == FR_OK)
                {
                    // Copy data onto file
                    uint32_t bytes_read = 0u;
                    test_fs = f_read(&g_fs_buffer_file, data, size, (UINT *)&bytes_read);
                    if ((test_fs == FR_OK) && (bytes_read == size))
                    {
                        // Close file
                        test_fs = f_close(&g_fs_buffer_file);
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
                return_value = FS_ERROR;
            }

            // Release Mutex anyway
            (void)xSemaphoreGive(g_fs_mutex);
        }
        else
        {
            return_value = FS_UNAVAILABLE;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
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
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    BaseType_t mutex_status;
    FRESULT test_fs;

    // Function Core
    if (file_size != NULL)
    {
        // First Acquire Mutex
        mutex_status = xSemaphoreTake(g_fs_mutex, 0u);
        if (mutex_status == pdTRUE)
        {
            // Open requested file
            test_fs = f_open(&g_fs_buffer_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
            if (test_fs == FR_OK)
            {
                // Get size
                *file_size = f_size(&g_fs_buffer_file);

                // Close file
                test_fs = f_close(&g_fs_buffer_file);
                if (test_fs != FR_OK)
                {
                    return_value = FS_ERROR;
                }
            }
            else
            {
                return_value = FS_ERROR;
            }

            // Release Mutex anyway
            (void)xSemaphoreGive(g_fs_mutex);
        }
        else
        {
            return_value = FS_UNAVAILABLE;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
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
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        // Link driver function
        fs_inst->driver.disk_initialize = NULL;
        fs_inst->driver.disk_status = NULL;
        fs_inst->driver.disk_read = NULL;
        fs_inst->driver.disk_write = NULL;
        fs_inst->driver.disk_ioctl = NULL;

        // We link driver functions to FATFS
        uint8_t test_hal = FATFS_UnLinkDriverEx(fs_inst->disk_path, 0u);
        if (test_hal != 0u)
        {
            return_value = FS_ERROR;
        }
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}