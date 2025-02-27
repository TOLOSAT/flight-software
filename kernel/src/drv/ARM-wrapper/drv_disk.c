/**
 * @file    drv_disk.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for disk driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

// TODO: Change the return values of some functions (use diskResult_t)

/******************************* Include Files *******************************/

#include "drv/disk.h"

#if !defined(CONFIG_FS_NONE)
#if defined(CONFIG_FS_RAM)
#include "drv/disk/diskdrv_ram.h"
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
#endif

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          DiskInitialize(diskByte_t disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk    Disk reference number
 * @retval      STA_NODISK if disk number is not valid or disk is not present
 * @retval      STA_NOINIT if disk initialisation failed
 * @retval      0 if disk initialization is a success
 */
diskStatus_t DiskInitialize(diskByte_t disk)
{
    // Init the disk
#if defined(CONFIG_FS_RAM)
    diskStatus_t res = RAM_DiskInit(disk);
#elif defined(CONFIG_FS_NONE)
    diskStatus_t res = RET_SUCCESSFUL;
    (void)(disk);
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif

    return res;
}

/**
 * @fn          DiskStatus(diskByte_t disk)
 * @brief       Function that returns disk status
 * @param[in]   disk    Driver reference number
 * @return      Disk Status
 */
diskStatus_t DiskStatus(diskByte_t disk)
{
#if defined(CONFIG_FS_RAM)
    return RAM_DiskStatus(disk);
#elif defined(CONFIG_FS_NONE)
    (void)(disk);
    return 0u;
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
}

/**
 * @fn          DiskRead(diskByte_t disk, diskByte_t *buff, diskWord_t sector, diskUint_t count)
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
diskResult_t DiskRead(diskByte_t disk, diskByte_t *buff, diskWord_t sector, diskUint_t count)
{
    diskResult_t res = RES_OK;

    // Read sector on the disk
    if (disk == DISK0_REF && count != 0)
    {
#if defined(CONFIG_FS_RAM)
        returnCode_t test_sd = RAM_DiskRead(disk, buff, sector, count);
#elif defined(CONFIG_FS_NONE)
        returnCode_t test_sd = RET_SUCCESSFUL;
        (void)(disk);
        (void)(buff);
        (void)(sector);
        (void)(count);
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
        if (test_sd != RET_SUCCESSFUL)
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
 * @fn          DiskWrite(diskByte_t disk, const diskByte_t *buff, diskWord_t sector, diskUint_t count)
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
diskResult_t DiskWrite(diskByte_t disk, const diskByte_t *buff, diskWord_t sector, diskUint_t count)
{
    diskResult_t res = RES_OK;

    // Write sector on the disk
    if (disk == DISK0_REF && count != 0)
    {
#if defined(CONFIG_FS_RAM)
        returnCode_t test_sd = RAM_DiskWrite(disk, buff, sector, count);
#elif defined(CONFIG_FS_NONE)
        returnCode_t test_sd = RET_SUCCESSFUL;
        (void)(disk);
        (void)(buff);
        (void)(sector);
        (void)(count);
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
        if (test_sd != RET_SUCCESSFUL)
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
 * @fn              DiskIoctl(diskByte_t disk, diskByte_t cmd, void *buff)
 * @brief           Function that operates a control over disk
 * @param[in]       disk    Disk reference number
 * @param[in]       cmd     Buffer of data to write on disk
 * @param[in,out]   buff    Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
diskResult_t DiskIoctl(diskByte_t disk, diskByte_t cmd, void *buff)
{
    diskResult_t res = RES_OK;

    // Perform ioctl on the disk
    if (disk == DISK0_REF)
    {
#if defined(CONFIG_FS_RAM)
        returnCode_t test_sd = RAM_DiskIoctl(disk, cmd, buff);
#elif defined(CONFIG_FS_NONE)
        returnCode_t test_sd = RET_SUCCESSFUL;
        (void)(disk);
        (void)(cmd);
        (void)(buff);
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
        if (test_sd != RET_SUCCESSFUL)
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
