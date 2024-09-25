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

#include "fs/fs.h"
#include "conf/fs_conf.h"
#include "drv/drv_disk.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

#if !defined(CONFIG_FS_NONE)
static kernelStatus_t FsTransferData(fileNo_t file_src, fileNo_t file_dest);
static FRESULT FsBuildFileSystem(void);
static FRESULT CreateParentDirectories(const char *path);
#endif /* CONFIG_FS_NONE */

/*************************** Variables Definitions ***************************/

#if !defined(CONFIG_FS_NONE)
/**
 * @var     fs_inst
 * @brief   File System instance declaration
 */
static fsInst_t IN_KERNEL_DATA_SECTION fs_inst = {0};
#endif /* CONFIG_FS_NONE */

/*************************** Functions Definitions ***************************/

/**
 * @fn              InitFs(void)
 * @brief           Function that initialise a FS
 * @retval          #KERNEL_ERROR if cannot create FS
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION InitFs(void)
{
#if defined(CONFIG_FS_NONE)
    // Always return successfull
    return KERNEL_SUCCESSFUL;
#else
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Link driver function
    fs_inst.driver.disk_initialize = DiskInitialize;
    fs_inst.driver.disk_status = DiskStatus;
    fs_inst.driver.disk_read = DiskRead;
    fs_inst.driver.disk_write = DiskWrite;
    fs_inst.driver.disk_ioctl = DiskIoctl;

    // We link driver functions to FATFS
    uint8_t test_fs = FATFS_LinkDriver(&fs_inst.driver, fs_inst.disk_path);
    if (test_fs != 0u)
    {
        return_value = KERNEL_ERROR;
    }
    else
    {
        // Then we mount the disk
        test_fs = f_mount(&fs_inst.file_system, "/", 1);
        if (test_fs == FR_NO_FILESYSTEM)
        {
            test_fs = FsBuildFileSystem();
        }

        // Check if mount went right
        if (test_fs == FR_OK)
        {
            // Now open all files
            fileNo_t file = 0u;
            while ((file < (fileNo_t)NB_FILES) && (test_fs == FR_OK))
            {
                test_fs = f_open(g_file_desc_table[file].temp_file, g_file_conf_table[file].name, g_file_conf_table[file].access_mode);
                if (return_value == KERNEL_SUCCESSFUL)
                {
                    // Then initialise mutex
                    g_file_desc_table[file].mutex = xSemaphoreCreateMutexStatic(g_file_conf_table[file].p_mutex_queue);
                    portENABLE_INTERRUPTS(); // WORKAROUND : FreeRTOS API disable interrupts by default if scheduler has not been started.
                    if (g_file_desc_table[file].mutex == NULL)
                    {
                        return_value = KERNEL_ERROR;
                    }
                }
                file++;
            }

            // Check if no error occured
            if (test_fs != FR_OK)
            {
                return_value = KERNEL_ERROR;
            }
        }
        else
        {
            return_value = KERNEL_ERROR;
        }
    }

    return return_value;
#endif
}

/**
 * @fn          FsWrite(fileNo_t file, data_t data, length_t length)
 * @brief       Function that write into a file of the fS
 * @param[in]   file File reference numero
 * @param[in]   data Pointer to data which will be written
 * @param[in]   length Length of data
 * @retval      #KERNEL_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #KERNEL_TIMEOUT if FS is already use by another thread
 * @retval      #KERNEL_ERROR if fatfs function has encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION FsWrite(fileNo_t file, data_t data, length_t length)
{
#if defined(CONFIG_FS_NONE)
    // Unused variables
    (void)(file);
    (void)(data);
    (void)(length);

    // Always return successfull
    return KERNEL_SUCCESSFUL;
#else
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (length != 0u) && (file < (fileNo_t)NB_FILES))
    {
        // Copy data onto file
        uint32_t bytes_written = 0u;
        test_fs = f_write(g_file_desc_table[file].temp_file, data, length, (UINT *)&bytes_written);
        if ((test_fs == FR_OK) && (bytes_written == length))
        {
            // Check if auto sync is enable
            if (g_file_conf_table[file].auto_sync == FS_AUTO_SYNC_ENABLE)
            {
                // Sync file
                test_fs = f_sync(g_file_desc_table[file].temp_file);
                if (test_fs != FR_OK)
                {
                    return_value = KERNEL_ERROR;
                }
            }
        }
        else
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn          FsRead(fileNo_t file, data_t data, length_t length)
 * @brief       Function that read from a file of the fS
 * @param[in]   file File reference numero
 * @param[out]  data Pointer to data which will be read
 * @param[in]   length Length of data
 * @retval      #KERNEL_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #KERNEL_TIMEOUT if FS is already use by another thread
 * @retval      #KERNEL_ERROR if fatfs function has encountered an error
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION FsRead(fileNo_t file, data_t data, length_t length)
{
#if defined(CONFIG_FS_NONE)
    // Unused variables
    (void)(file);
    (void)(data);
    (void)(length);

    // Always return successfull
    return KERNEL_SUCCESSFUL;
#else
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    FRESULT test_fs;

    // Function Core
    if ((data != NULL) && (length != 0u) && (file < (fileNo_t)NB_FILES))
    {
        // Copy data onto file
        uint32_t bytes_read = 0u;
        test_fs = f_read(g_file_desc_table[file].temp_file, data, length, (UINT *)&bytes_read);
        if ((test_fs != FR_OK) || (bytes_read != length))
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
#endif
}

/**
 * @fn              FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to manage a file
 * @param[in]       file File reference numero
 * @param[in]       cmd IO Control command
 * @param[in,out]   data IO Control command
 * @param[in]       data_size IO Control data length
 * @retval          #KERNEL_INVALID_PARAM if a pointer is null
 * @retval          #KERNEL_ERROR if IO control failed
 * @retval          #KERNEL_SUCCESSFUL else
 */
kernelStatus_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size)
{
#if defined(CONFIG_FS_NONE)
    // Unused variables
    (void)(file);
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Always return successfull
    return KERNEL_SUCCESSFUL;
#else
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    FRESULT test_fs = FR_OK;

    // Function Core
    switch (cmd)
    {
    case FS_IOCTL_GET_SIZE:
        if ((data != NULL) && (data_size == sizeof(length_t)))
        {
            length_t *file_size = (length_t *)data;
            *file_size = f_size(g_file_desc_table[file].temp_file);
        }
        else
        {
            return_value = KERNEL_INVALID_PARAM;
        }
        break;
    case FS_IOCTL_SEEK:
        if ((data != NULL) && (data_size == sizeof(length_t)))
        {
            length_t target_pointer = *(length_t *)data;
            // Move the read/write pointer to the desired offset
            test_fs = f_lseek(g_file_desc_table[file].temp_file, target_pointer);
            if (test_fs == FR_OK)
            {
                // Check if it has been move correctly (otherwise it means either disk full 
                // or end-of-file for read-only files)
                length_t current_pointer = f_tell(g_file_desc_table[file].temp_file);
                if (current_pointer != target_pointer)
                {
                    return_value = KERNEL_ERROR;
                }
            }
            else
            {
                return_value = KERNEL_ERROR;
            }
        }
        else
        {
            return_value = KERNEL_INVALID_PARAM;
        }
        break;
    case FS_IOCTL_SYNC:
        // Synchronise the temporary data (in RAM) with the disk
        test_fs = f_sync(g_file_desc_table[file].temp_file);
        if (test_fs != FR_OK)
        {
            return_value = KERNEL_ERROR;
        }
        break;
    case FS_IOCTL_TRANSFER_DATA:
        if ((data != NULL) && (data_size == sizeof(length_t)))
        {
            fileNo_t file_dest = *(fileNo_t *)data;
            // Transfer the content of current file to the destination file
            return_value = FsTransferData(file, file_dest);
        }
        else
        {
            return_value = KERNEL_INVALID_PARAM;
        }
        break;
    default:
        return_value = KERNEL_INVALID_PARAM;
        break;
    }

    return return_value;
#endif
}

/**
 * @fn          DeinitFs(void)
 * @brief       Function that desinit the disk (and FS) connection and puts defaults parameters
 * @retval      #KERNEL_ERROR if cannot close file system properly
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DeinitFs(void)
{
#if defined(CONFIG_FS_NONE)
    // Always return successfull
    return KERNEL_SUCCESSFUL;
#else
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // First close every file
    uint8_t test_fs = FR_OK;
    fileNo_t file = 0u;
    while ((file < (fileNo_t)NB_FILES) && (test_fs == FR_OK))
    {
        test_fs = f_close(g_file_desc_table[file].temp_file);
        file++;
    }

    // Check if no error occured
    if (test_fs == FR_OK)
    {
        // Then unmount drive
        test_fs = f_unmount("/");
        if (test_fs == 0u)
        {
            // Link driver function
            fs_inst.driver.disk_initialize = NULL;
            fs_inst.driver.disk_status = NULL;
            fs_inst.driver.disk_read = NULL;
            fs_inst.driver.disk_write = NULL;
            fs_inst.driver.disk_ioctl = NULL;

            // We unlink driver functions to FATFS
            test_fs = FATFS_UnLinkDriverEx(fs_inst.disk_path, 0u);
            if (test_fs != 0u)
            {
                return_value = KERNEL_ERROR;
            }
        }
        else
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_ERROR;
    }

    return return_value;
#endif
}

#if !defined(CONFIG_FS_NONE)
/**
 * @fn          FsTransferData(fileNo_t file_src, fileNo_t file_dest)
 * @brief       Function that transfer content from one file to another
 * @param[in]   file_src Source file
 * @param[in]   file_dest Destination file
 * @return      #KERNEL_INVALID_PARAM if the destination file is the source file
 * @return      #KERNEL_ERROR if the transfer went wrong
 * @return      #KERNEL_SUCCESSFUL else
 * 
 * This function will erase the destination file and write source file data in
 * there. Source file will be left empty.
 */
static kernelStatus_t IN_KERNEL_TEXT_SECTION FsTransferData(fileNo_t file_src, fileNo_t file_dest)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;
    FRESULT test_fs;

    if (file_dest != file_src)
    {
        // First close the files in order to avoid issues when renaming and deleting files
        test_fs = f_close(g_file_desc_table[file_src].temp_file);
        if (test_fs == FR_OK)
        {
            test_fs = f_close(g_file_desc_table[file_dest].temp_file);
        }

        // Remove the old console file (we keep only one old file)
        if (test_fs == FR_OK)
        {
            test_fs = f_unlink(g_file_conf_table[file_dest].name);
        }

        // Then rename the file
        if (test_fs == FR_OK)
        {
            test_fs = f_rename(g_file_conf_table[file_src].name, g_file_conf_table[file_dest].name);
        }

        // Then we can open the console files again
        if (test_fs == FR_OK)
        {
            test_fs = f_open(g_file_desc_table[file_src].temp_file, g_file_conf_table[file_src].name, g_file_conf_table[file_src].access_mode);
        }
        if (test_fs == FR_OK)
        {
            test_fs = f_open(g_file_desc_table[file_dest].temp_file, g_file_conf_table[file_dest].name, g_file_conf_table[file_dest].access_mode);
        }

        // Check if the process went right
        if (test_fs != FR_OK)
        {
            return_value = KERNEL_ERROR;
        }
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          FsBuildFileSystem(void)
 * @brief       Function that rebuild the file system if not present on the drive
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @return      FRESULT
 *
 * @warning This function will recreate a file system so it will potentially erase data if any
 */
static FRESULT IN_KERNEL_TEXT_SECTION FsBuildFileSystem(void)
{
    // Variable initialisation
    FRESULT return_value = FR_OK;
    uint8_t work[FF_MAX_SS] = {0}; // cppcheck-suppress misra-c2012-18.8; False positive
    fileNo_t file = 0u;

    // Function Core
    return_value = f_mkfs("/", 0, work, FF_MAX_SS);

    // Now create parent directories for every file
    while ((return_value == FR_OK) && (file < (fileNo_t)NB_FILES))
    {
        return_value = CreateParentDirectories(g_file_conf_table[file].name);
        file++;
    }

    return return_value;
}

/**
 * @fn          CreateParentDirectories(const char *path)
 * @brief       Function that create a directory for every dir in a file path
 * @param[in]   fs_inst Instance that contains FS parameters and driver
 * @return      FRESULT
 */
static FRESULT IN_KERNEL_TEXT_SECTION CreateParentDirectories(const char *path)
{
    // Variable initialization
    FRESULT res = FR_OK;
    char tmp_path[FF_MAX_LFN]; // cppcheck-suppress misra-c2012-18.8; False positive
    uint32_t length = 0u;
    uint32_t i = 0u;

    // First copy the path in the buffer
    (void)strcpy(tmp_path, path);
    length = strlen(tmp_path);

    // Iterate over the path and create each missing directory
    while ((i < length) && ((res == FR_OK) || (res == FR_EXIST)))
    {
        if (tmp_path[i] == '/')
        {
            // Temporarily replace '/' with '\0' to create the directory up to this point
            tmp_path[i] = '\0';

            // Create dir
            res = f_mkdir(tmp_path);

            // Restore the '/' character
            tmp_path[i] = '/';
        }
        i++;
    }

    // Just if FR_EXIST it means the dir already exists so we return FR_OK for compatibility
    if (res == FR_EXIST)
    {
        res = FR_OK;
    }

    return res;
}
#endif /* CONFIG_FS_NONE */