/**
 * @file    drv_disk.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT Disk IO functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "file-system/drv_disk.h"

#if defined(CONFIG_FS_SD)
#include "memory/memdrv_sd.h"
#else
#error Please #define CONFIG_FS_SD
#endif

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          DiskInitialize(BYTE disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk Disk reference number
 * @retval      STA_NOINIT if disk number is not valid
 * @retval      STA_NODISK if disk is not available
 * @retval      0 if disk initialization is a success
 */
DSTATUS DiskInitialize(BYTE disk)
{
    DSTATUS res = STA_NOINIT;

    // Init SD Card
#if defined(CONFIG_FS_SD)
    res = SD_GetStatus(disk);
#else
#error Please #define CONFIG_FS_SD
#endif

    return res;
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk Driver reference number
 * @return      Disk Status
 */
DSTATUS DiskStatus(BYTE disk)
{
#if defined(CONFIG_FS_SD)
    return SD_GetStatus(disk);
#else
#error Please #define CONFIG_FS_SD
#endif
}

/**
 * @fn          DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that reads inside disk
 * @param[in]   disk Disk reference number
 * @param[out]  buff Buffer where data goes after reading
 * @param[in]   sector First sector address
 * @param[in]   count Number of sector to read
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_ERROR if reading has encountered an error
 * @retval      RES_OK else
 */
DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_OK;

    // Read sector in SD card
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_ReadBlocks(disk, buff, sector, count);
#else
#error Please #define CONFIG_FS_SD
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
}

/**
 * @fn          DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
 * @brief       Function that writes inside disk
 * @param[in]   disk Disk reference number
 * @param[in]   buff Buffer of data to write on disk
 * @param[in]   sector First sector address
 * @param[in]   count Number of sector to write
 * @retval      RES_PARERR if disk is not DISK0_REF or count is null
 * @retval      RES_NOTRDY if disk is not ready
 * @retval      RES_WRPRT if disk is protected against reading
 * @retval      RES_ERROR if writing has encountered an error
 * @retval      RES_OK else
 */
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
{
    DRESULT res = RES_OK;

    // Write sector in SD card
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_WriteBlocks(disk, buff, sector, count);
#else
#error Please #define CONFIG_FS_SD
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
}

/**
 * @fn              DiskIoctl(BYTE disk, BYTE cmd, void *buff)
 * @brief           Function that operates a control over disk
 * @param[in]       disk Disk reference number
 * @param[in]       cmd Buffer of data to write on disk
 * @param[in,out]   buff Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF or count is null
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff)
{
    DRESULT res = RES_OK;

    // SD Card disk IOCTL
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_Ioctl(disk, cmd, buff);
#else
#error Please #define CONFIG_FS_SD
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
}
