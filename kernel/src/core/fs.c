/**
 * @file    fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT File System functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "core/fs.h"
#include "core/mutex.h"
#include "drv/memories.h"
#include "drv/others/drv_rtc.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define DISK0_REF 0x00u /**< Disk0 reference */

/*************************** Functions Declarations **************************/

#if defined(CONFIG_FS_ENABLED)
static returnCode_t FsTransferData(fileNo_t file_src, fileNo_t file_dest);
static FRESULT FsBuildFileSystem(void);
static FRESULT CreateParentDirectories(const char *path);

#if FF_FS_REENTRANT
extern int ff_mutex_create(int vol);
extern void ff_mutex_delete(int vol);
extern int ff_mutex_take(int vol);
extern void ff_mutex_give(int vol);
#endif /* FF_FS_REENTRANT */

#if !FF_FS_NORTC
extern uint32_t get_fattime(void);
#endif /* !FF_FS_NORTC */

static DSTATUS DiskInitialize(BYTE disk);
static DSTATUS DiskStatus(BYTE disk);
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff);
#endif /* CONFIG_FS_ENABLED */

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_FS_ENABLED)
static mutexHandle_t fs_mutex    = { 0 };
static bool fs_mutex_initialised = false;
static fsInst_t fs_inst          = { 0 };
#endif /* CONFIG_FS_ENABLED */

/*************************** Functions Definitions ***************************/

/**
 * @fn              InitFs(void)
 * @brief           Function that initialise a FS
 * @return          Nothing
 */
void InitFs(void)
{
#if defined(CONFIG_FS_ENABLED)
    // Link driver function
    fs_inst.driver.disk_initialize = DiskInitialize;
    fs_inst.driver.disk_status     = DiskStatus;
    fs_inst.driver.disk_read       = DiskRead;
    fs_inst.driver.disk_write      = DiskWrite;
    fs_inst.driver.disk_ioctl      = DiskIoctl;

    // We link driver functions to FATFS
    uint8_t test_fs = FATFS_LinkDriver(&fs_inst.driver, fs_inst.disk_path);
    if (test_fs != 0u)
    {
        KernelPanic();
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
            fileNo_t file = 1u;
            while ((FILE_CONF(file).file != NO_FILE) && (test_fs == FR_OK))
            {
                test_fs = f_open(FILE_DESC(file).temp_file, FILE_CONF(file).name, FILE_CONF(file).access_mode);

                // If the file and/or path does not exist creates it
                if ((test_fs == FR_NO_FILE) || (test_fs == FR_NO_PATH))
                {
                    test_fs = CreateParentDirectories(FILE_CONF(file).name);
                    if (test_fs == FR_OK)
                    {
                        test_fs = f_open(FILE_DESC(file).temp_file, FILE_CONF(file).name, FILE_CONF(file).access_mode | FA_CREATE_NEW);
                    }
                }

                // Indicates the file is initialised
                FILE_DESC(file).status = DESC_USED;
                file++;
            }

            // Check if no error occured
            if (test_fs != FR_OK)
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
#endif
}

/**
 * @fn      CreateFsMutexes(void)
 * @brief   Function that allows to postpone mutex initilisation when other mutexes will be initialised.
 */
void CreateFsMutexes(void)
{
#if defined(CONFIG_FS_ENABLED)
    static mutexQueue_t fs_mutex_queue = { 0 };

    // Initialise mutex for the filesystem
    fs_mutex = xSemaphoreCreateMutexStatic(&fs_mutex_queue);
    if (fs_mutex == NULL)
    {
        KernelPanic();
    }
    else
    {
        fs_mutex_initialised = true;
    }
#endif
}

/**
 * @fn          FsWrite(fileNo_t file, data_t data, length_t length)
 * @brief       Function that write into a file of the fS
 * @param[in]   file    File reference numero
 * @param[in]   data    Pointer to data which will be written
 * @param[in]   length  Length of data
 * @retval      #RET_INVALID_PARAM if the file is not valid
 * @retval      #RET_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t FsWrite(fileNo_t file, data_t data, length_t length)
{
#if defined(CONFIG_FS_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs;

    // Check parameter(s)
    if ((data != NULL) && (length != 0u) && (IS_A_VALID_FILE(file)))
    {
        UINT bytes_written = 0u;
        // Copy data onto file
        test_fs = f_write(FILE_DESC(file).temp_file, data, length, &bytes_written);
        if ((test_fs == FR_OK) && (bytes_written == length))
        {
            // Check if auto sync is enable
            if (FILE_CONF(file).auto_sync == FS_AUTO_SYNC_ENABLE)
            {
                // Sync file
                test_fs = f_sync(FILE_DESC(file).temp_file);
                if (test_fs != FR_OK)
                {
                    KernelPanic();
                }
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
#else
    // Unused variables
    (void)(file);
    (void)(data);
    (void)(length);

    // Always return successfull
    return RET_SUCCESSFUL;
#endif
}

/**
 * @fn          FsRead(fileNo_t file, data_t data, length_t length)
 * @brief       Function that read from a file of the fS
 * @param[in]   file    File reference numero
 * @param[out]  data    Pointer to data which will be read
 * @param[in]   length  Length of data
 * @retval      #RET_INVALID_PARAM if the file is not valid
 * @retval      #RET_INVALID_PARAM if a parameter is null pointer or data length is null
 * @retval      #RET_NOT_AVAILABLE if the section of the file asked does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t FsRead(fileNo_t file, data_t data, length_t length)
{
#if defined(CONFIG_FS_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs;

    // Check parameter(s)
    if ((data != NULL) && (length != 0u) && (IS_A_VALID_FILE(file)))
    {
        // Get file size and read/write pointer position
        uint32_t current_size = f_size(FILE_DESC(file).temp_file);
        uint32_t pointer_pos  = f_tell(FILE_DESC(file).temp_file);

        // Check read is possible
        if (length <= (current_size - pointer_pos))
        {
            uint32_t bytes_read = 0u;
            // Copy data onto file
            test_fs = f_read(FILE_DESC(file).temp_file, data, length, (UINT *)&bytes_read);
            if ((test_fs != FR_OK) || (bytes_read != length))
            {
                KernelPanic();
            }
        }
        else
        {
            return_value = RET_NOT_AVAILABLE;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
#else
    // Unused variables
    (void)(file);
    (void)(data);
    (void)(length);

    // Always return successfull
    return RET_SUCCESSFUL;
#endif
}

/**
 * @fn              FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that adds advanced control to manage a file
 * @param[in]       file        File reference numero
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data length
 * @retval          #RET_INVALID_PARAM if the file is not valid
 * @retval          #RET_INVALID_PARAM if a pointer is null
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t FsIoctl(fileNo_t file, uint32_t cmd, void *data, uint32_t data_size)
{
#if defined(CONFIG_FS_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs           = FR_OK;

    // Check parameter(s)
    if (IS_A_VALID_FILE(file))
    {
        // Then do IOCTL depending on the command
        switch (cmd)
        {
            case IOCTL_FS_GET_SIZE :
                if ((data != NULL) && (data_size == sizeof(length_t)))
                {
                    length_t *file_size = (length_t *)data;

                    *file_size = f_size(FILE_DESC(file).temp_file);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_FS_SEEK :
                if ((data != NULL) && (data_size == sizeof(length_t)))
                {
                    length_t target_pointer = *(length_t *)data;
                    // Move the read/write pointer to the desired offset
                    test_fs = f_lseek(FILE_DESC(file).temp_file, target_pointer);
                    if (test_fs == FR_OK)
                    {
                        // Check if it has been move correctly (otherwise it means either disk full
                        // or end-of-file for read-only files)
                        length_t current_pointer = f_tell(FILE_DESC(file).temp_file);
                        if (current_pointer != target_pointer)
                        {
                            KernelPanic();
                        }
                    }
                    else
                    {
                        KernelPanic();
                    }
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_FS_SYNC :
                // Synchronise the temporary data (in RAM) with the disk
                test_fs = f_sync(FILE_DESC(file).temp_file);
                if (test_fs != FR_OK)
                {
                    KernelPanic();
                }
                break;
            case IOCTL_FS_TRANSFER_DATA :
                if ((data != NULL) && (data_size == sizeof(length_t)))
                {
                    fileNo_t file_dest = *(fileNo_t *)data;
                    // Transfer the content of current file to the destination file
                    return_value = FsTransferData(file, file_dest);
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
#else
    // Unused variables
    (void)(file);
    (void)(cmd);
    (void)(data);
    (void)(data_size);

    // Always return successfull
    return RET_SUCCESSFUL;
#endif
}

/**
 * @fn          DeinitFs(void)
 * @brief       Function that desinit the disk (and FS) connection and puts defaults parameters
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t DeinitFs(void)
{
#if defined(CONFIG_FS_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;

    // First close every file
    uint8_t test_fs = FR_OK;
    fileNo_t file   = 0u;
    while ((IS_A_VALID_FILE(file)) && (test_fs == FR_OK))
    {
        test_fs = f_close(FILE_DESC(file).temp_file);
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
            fs_inst.driver.disk_status     = NULL;
            fs_inst.driver.disk_read       = NULL;
            fs_inst.driver.disk_write      = NULL;
            fs_inst.driver.disk_ioctl      = NULL;

            // We unlink driver functions to FATFS
            test_fs = FATFS_UnLinkDriverEx(fs_inst.disk_path, 0u);
            if (test_fs != 0u)
            {
                KernelPanic();
            }
        }
        else
        {
            KernelPanic();
        }
    }
    else
    {
        KernelPanic();
    }

    return return_value;
#else
    // Always return successfull
    return RET_SUCCESSFUL;
#endif
}

#if defined(CONFIG_FS_ENABLED)
/**
 * @fn          FsTransferData(fileNo_t file_src, fileNo_t file_dest)
 * @brief       Function that transfer content from one file to another
 * @param[in]   file_src    Source file
 * @param[in]   file_dest   Destination file
 * @return      #RET_INVALID_PARAM if the destination file is the source file
 * @return      #RET_SUCCESSFUL else
 *
 * This function will erase the destination file and write source file data in
 * there. Source file will be left empty.
 */
static returnCode_t FsTransferData(fileNo_t file_src, fileNo_t file_dest)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs;

    // Check parameter(s)
    if (file_dest != file_src)
    {
        // First close the files in order to avoid issues when renaming and deleting files
        test_fs = f_close(FILE_DESC(file_src).temp_file);
        if (test_fs == FR_OK)
        {
            test_fs = f_close(FILE_DESC(file_dest).temp_file);
        }

        // Remove the old console file (we keep only one old file)
        if (test_fs == FR_OK)
        {
            test_fs = f_unlink(FILE_CONF(file_dest).name);
        }

        // Then rename the file
        if (test_fs == FR_OK)
        {
            test_fs = f_rename(FILE_CONF(file_src).name, FILE_CONF(file_dest).name);
        }

        // Then we can open the console files again
        if (test_fs == FR_OK)
        {
            test_fs = f_open(FILE_DESC(file_src).temp_file, FILE_CONF(file_src).name, FILE_CONF(file_src).access_mode);
        }
        if (test_fs == FR_OK)
        {
            test_fs = f_open(FILE_DESC(file_dest).temp_file, FILE_CONF(file_dest).name, FILE_CONF(file_dest).access_mode);
        }

        // Check if the process went right
        if (test_fs != FR_OK)
        {
            KernelPanic();
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
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
static FRESULT FsBuildFileSystem(void)
{
    FRESULT return_value    = FR_OK;
    uint8_t work[FF_MAX_SS] = { 0 };
    fileNo_t file           = 1u;

    // Start by making a filesystem
    return_value = f_mkfs("/", 0, work, FF_MAX_SS);

    // Now create parent directories for every file
    while ((return_value == FR_OK) && (IS_A_VALID_FILE(file)))
    {
        return_value = CreateParentDirectories(FILE_CONF(file).name);
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
static FRESULT CreateParentDirectories(const char *path)
{
    FRESULT res = FR_OK;
    char tmp_path[FF_MAX_LFN];
    uint32_t length = 0u;
    uint32_t i      = 0u;

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

#if FF_FS_REENTRANT
/**
 * @fn          ff_mutex_create(int vol)
 * @brief       This function is called in f_mount function to create a new mutex for the volume.
 * @param[in]   vol Volume ID
 * @retval      1 Function succeeded
 * @retval      0 Volume is not correct
 */
int ff_mutex_create(int vol)
{
    int ret = 1;

    if (vol != 0)
    {
        ret = 0;
    }

    return ret;
}

/**
 * @fn          ff_mutex_delete(int vol)
 * @brief       This function is called in f_mount function to delete a mutex of the volume created.
 * @param[in]   vol Volume ID
 * @return      Nothing
 */
void ff_mutex_delete(int vol)
{
    // Check Volume
    if (vol == 0)
    {
        if (fs_mutex_initialised)
        {
            vSemaphoreDelete(fs_mutex);
        }
    }
}

/**
 * @fn          ff_mutex_take(int vol)
 * @brief       This function is called on enter file functions to lock the volume.
 * @param[in]   vol Volume ID
 * @retval      1 Succeeded
 * @retval      0 Timeout/Error
 */
int ff_mutex_take(int vol)
{
    int ret = 1;

    // Check Volume
    if (vol == 0)
    {
        if (fs_mutex_initialised)
        {
            BaseType_t mutex_status = xSemaphoreTake(fs_mutex, portMAX_DELAY);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
    else
    {
        ret = 0;
    }

    return ret;
}

/**
 * @fn          ff_mutex_take(int vol)
 * @brief       This function is called on leave file functions to unlock the volume.
 * @param[in]   vol Volume ID
 * @return      Nothing
 */
void ff_mutex_give(int vol)
{
    // Check Volume
    if (vol == 0)
    {
        if (fs_mutex_initialised)
        {
            BaseType_t mutex_status = xSemaphoreGive(fs_mutex);
            if (mutex_status != pdTRUE)
            {
                KernelPanic();
            }
        }
    }
}
#endif /* FF_FS_REENTRANT */

#if !FF_FS_NORTC
/**
 * @fn      get_fattime(void)
 * @brief   Gets Time from RTC
 * @return  Time
 */
uint32_t get_fattime(void)
{
    uint32_t time      = 0u;
    rtcTime_t rtc_time = { 0 };

    // Get time
    returnCode_t test_val = RtcGetTime(&rtc_time);
    if (test_val == RET_SUCCESSFUL)
    {
        time = (((uint32_t)(rtc_time.year + 20u) & 0x7Fu) << 25) | // Year origin from the 1980 (0..127, e.g. 37 for 2017)
               (((uint32_t)rtc_time.month & 0x0Fu) << 21) |        // Month (1..12)
               (((uint32_t)rtc_time.day & 0x1Fu) << 16) |          // Day of the month (1..31)
               (((uint32_t)rtc_time.hour & 0x1Fu) << 11) |         // Hour (0..23)
               (((uint32_t)rtc_time.minute & 0x3Fu) << 5) |        // Minute (0..59)
               (((uint32_t)(rtc_time.second / 2u)) & 0x1Fu);       // Second / 2 (0..29, e.g. 25 for 50)
    }
    else
    {
        KernelPanic();
    }

    return time;
}
#endif /* !FF_FS_NORTC */

/**
 * @fn          DiskInitialize(BYTE disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk    Disk reference number
 * @retval      STA_NODISK if disk number is not valid or disk is not present
 * @retval      STA_NOINIT if disk initialisation failed
 * @retval      0 if disk initialization is a success
 */
static DSTATUS DiskInitialize(BYTE disk)
{
    (void)(disk);
    return RES_OK;
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk    Driver reference number
 * @return      Disk Status
 */
static DSTATUS DiskStatus(BYTE disk)
{
    DSTATUS status = STA_NODISK;

    if (disk == DISK0_REF)
    {
        // Get status
        memoryStatus_t memory_status;
        returnCode_t test_val = MemoryIoctl(g_fs_mem, IOCTL_MEMORY_GET_STATUS, &memory_status, sizeof(memoryStatus_t));
        if (test_val == RET_SUCCESSFUL)
        {
            switch (memory_status)
            {
                case MEMORY_NO_INIT :
                    status = STA_NOINIT;
                    break;
                case MEMORY_NO_DISK :
                    status = STA_NODISK;
                    break;
                case MEMORY_READY :
                    status = 0x00;
                    break;
                default :
                    status = STA_NODISK;
                    break;
            }
        }
    }

    return status;
}

/**
 * @fn          DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that reads inside disk
 * @param[in]   disk    Disk reference number
 * @param[out]  buff    Buffer where data goes after reading
 * @param[in]   sector  First sector address
 * @param[in]   count   Number of sector to read
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_ERROR if reading has encountered an error
 * @retval      RES_OK else
 */
static DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_OK;

    // Read sector on the disk
    if ((disk == DISK0_REF) && (count != 0))
    {
        returnCode_t test_val = MemoryRead(g_fs_mem, sector, buff, count);
        if (test_val != RET_SUCCESSFUL)
        {
            res = RES_ERROR;
        }
    }
    else
    {
        res = RES_PARERR;
    }

    return res;
}

/**
 * @fn          DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that writes inside disk
 * @param[in]   disk    Disk reference number
 * @param[in]   buff    Buffer of data to write on disk
 * @param[in]   sector  First sector address
 * @param[in]   count   Number of sector to write
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_WRPRT if disk is protected against reading
 * @retval      RES_ERROR if writing has encountered an error
 * @retval      RES_OK else
 */
static DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_OK;

    // Write sector on the disk
    if ((disk == DISK0_REF) && (count != 0))
    {
        returnCode_t test_val = MemoryWrite(g_fs_mem, sector, (data_t)buff, count);
        if (test_val != RET_SUCCESSFUL)
        {
            res = RES_ERROR;
        }
    }
    else
    {
        res = RES_PARERR;
    }

    return res;
}

/**
 * @fn              DiskIoctl(BYTE disk, BYTE cmd, void *buff)
 * @brief           Function that operates a control over disk
 * @param[in]       disk    Disk reference number
 * @param[in]       cmd     Buffer of data to write on disk
 * @param[in,out]   buff    Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
static DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff)
{
    DRESULT res = RES_OK;

    // Perform ioctl on the disk
    if (disk == DISK0_REF)
    {
        // Conversion between IOCTL
        uint32_t memory_ioctl_cmd  = 0u;
        uint32_t memory_ioctl_size = 0u;
        switch (cmd)
        {
            case CTRL_SYNC :
                memory_ioctl_cmd  = IOCTL_MEMORY_SYNC;
                memory_ioctl_size = 0u;
                break;
            case GET_SECTOR_COUNT :
                memory_ioctl_cmd  = IOCTL_MEMORY_GET_SECTOR_COUNT;
                memory_ioctl_size = sizeof(memorySectorCount_t);
                break;
            case GET_SECTOR_SIZE :
                memory_ioctl_cmd  = IOCTL_MEMORY_GET_SECTOR_SIZE;
                memory_ioctl_size = sizeof(memorySectorSize_t);
                break;
            case GET_BLOCK_SIZE :
                memory_ioctl_cmd  = IOCTL_MEMORY_GET_BLOCK_SIZE;
                memory_ioctl_size = sizeof(memoryBlockSize_t);
                break;
            default :
                res = RES_PARERR;
                break;
        }

        if (res == RES_OK)
        {
            // Do the IOCTL
            returnCode_t test_val = MemoryIoctl(g_fs_mem, memory_ioctl_cmd, buff, memory_ioctl_size);
            if (test_val != RET_SUCCESSFUL)
            {
                res = RES_ERROR;
            }
        }
    }
    else
    {
        res = RES_PARERR;
    }

    return res;
}

#endif /* CONFIG_FS_ENABLED */