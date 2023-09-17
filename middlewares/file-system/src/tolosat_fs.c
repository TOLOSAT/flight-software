/**
 * @file    tolosat_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_fs.h"
#include "conf/fs_conf.h"
#include "diskio.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_buffer_file
 * @brief   Buffer file used when FsWrite, FsRead, or FsIoCtl are used
 */
FIL g_buffer_file = {0};

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
            if (test_hal != 0u)
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
 * @fn          FsWrite(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
 * @brief       Function that write into a file of the fS
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @param[in]   fileno File reference numero
 * @param[in]   offset Offset from where data will be written
 * @param[in]   data Pointer to data which will be written
 * @param[in]   size Size of data
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_ERROR if an error occured when using fatfs functions
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t FsWrite(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
    // Unused Variable
    (void)(fs_inst);

    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((fs_inst != NULL) && (data != NULL) && (size != 0u) && (fileno < MAX_NB_FILES_PER_DEVICES))
    {
        // Open requested file
        test_fs = f_open(&g_buffer_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
        if (test_fs == FR_OK)
        {
            // Places the write pointer in the right place
            test_fs = f_lseek(&g_buffer_file, offset);
            if (test_fs == FR_OK)
            {
                // Copy data onto file
                uint32_t bytes_written = 0u;
                test_fs = f_write(&g_buffer_file, data, size, (UINT *)&bytes_written);
                if ((test_fs == FR_OK) && (bytes_written == size))
                {
                    // Close data
                    test_fs = f_close(&g_buffer_file);
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
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          FsRead(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
 * @brief       Function that read from a file of the fS
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @param[in]   fileno File reference numero
 * @param[in]   offset Offset from where data will be read
 * @param[out]  data Pointer to data which will be read
 * @param[in]   size Size of data
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t FsRead(fsInst_t *fs_inst, fsFileno_t fileno, fsSize_t offset, fsData_t *data, fsSize_t size)
{
    // Unused Variable
    (void)(fs_inst);
    
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((fs_inst != NULL) && (data != NULL) && (size != 0u) && (fileno < MAX_NB_FILES_PER_DEVICES))
    {
        // Open requested file
        test_fs = f_open(&g_buffer_file, g_files_conf[SD0][fileno].name, g_files_conf[SD0][fileno].access_mode);
        if (test_fs == FR_OK)
        {
            // Places the write pointer in the right place
            test_fs = f_lseek(&g_buffer_file, offset);
            if (test_fs == FR_OK)
            {
                // Copy data onto file
                uint32_t bytes_read = 0u;
                test_fs = f_read(&g_buffer_file, data, size, (UINT *)&bytes_read);
                if ((test_fs == FR_OK) && (bytes_read == size))
                {
                    // Close data
                    test_fs = f_close(&g_buffer_file);
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
    }
    else
    {
        return_value = FS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          FsIoCtl(fsInst_t *fs_inst, fsIoCtlCmd_t io_cmd)
 * @brief       Function that adds advanced control to the FS
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @param[in]   io_cmd IO Control command struct (including data)
 * @retval      #FS_INVALID_PARAM if a parameter is null pointer or data size is null
 * @retval      #FS_SUCCESSFUL else
 */
fsStatus_t FsIoCtl(fsInst_t *fs_inst, fsIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    fsStatus_t return_value = FS_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        /* To Do */
        (void)(fs_inst);
        (void)(io_cmd);
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