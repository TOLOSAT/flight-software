/**
 * @file    drv_disk.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for disk driver functions
 * @date    17/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "drv/drv_disk.h"

#if !defined(CONFIG_FS_NONE)
#if defined(CONFIG_FS_SD)
#include "drv/disk/diskdrv_sd.h"
#elif defined(CONFIG_FS_SPISD)
#include "drv/disk/diskdrv_spisd.h"
#elif defined(CONFIG_FS_RAM)
#include "drv/disk/diskdrv_ram.h"
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
#endif

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          DiskInitialize(BYTE disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk    Disk reference number
 * @retval      STA_NOINIT if disk number is not valid
 * @retval      STA_NODISK if disk is not available
 * @retval      0 if disk initialization is a success
 */
DSTATUS DiskInitialize(BYTE disk)
{
#if defined(CONFIG_FS_NONE)
    (void)(disk);
    return RES_OK;
#else
    // Variable Initialisation
    DSTATUS res = STA_NOINIT;

    // Function Core
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_DiskInit(disk);
#elif defined(CONFIG_FS_SPISD)
    returnCode_t test_sd = SpiSD_DiskInit(disk);
#elif defined(CONFIG_FS_RAM)
    returnCode_t test_sd = RAM_DiskInit(disk);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    if (test_sd == RET_SUCCESSFUL)
    {
#if defined(CONFIG_FS_SD)
        res = SD_DiskStatus(disk);
#elif defined(CONFIG_FS_SPISD)
        res = SpiSD_DiskStatus(disk);
#elif defined(CONFIG_FS_RAM)
        res = RAM_DiskStatus(disk);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    }

    return res;
#endif /* CONFIG_FS_NONE */
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk    Driver reference number
 * @return      Disk Status
 */
DSTATUS DiskStatus(BYTE disk)
{
#if defined(CONFIG_FS_NONE)
    (void)(disk);
    return RES_OK;
#else
#if defined(CONFIG_FS_SD)
    return SD_DiskStatus(disk);
#elif defined(CONFIG_FS_SPISD)
    return SpiSD_DiskStatus(disk);
#elif defined(CONFIG_FS_RAM)
    return RAM_DiskStatus(disk);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
#endif /* CONFIG_FS_NONE */
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
DRESULT DiskRead(BYTE disk, BYTE *buff, DWORD sector, UINT count)
{
#if defined(CONFIG_FS_NONE)
    (void)(disk);
    (void)(buff);
    (void)(sector);
    (void)(count);
    return RES_OK;
#else
    // Variable Initialisation
    DRESULT res = RES_OK ;

    // Function Core
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_DiskRead(disk, buff, sector, count);
#elif defined(CONFIG_FS_SPISD)
    returnCode_t test_sd = SpiSD_DiskRead(disk, buff, sector, count);
#elif defined(CONFIG_FS_RAM)
    returnCode_t test_sd = RAM_DiskRead(disk, buff, sector, count);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
#endif /* CONFIG_FS_NONE */
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
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
{
#if defined(CONFIG_FS_NONE)
    (void)(disk);
    (void)(buff);
    (void)(sector);
    (void)(count);
    return RES_OK;
#else
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_DiskWrite(disk, buff, sector, count);
#elif defined(CONFIG_FS_SPISD)
    returnCode_t test_sd = SpiSD_DiskWrite(disk, buff, sector, count);
#elif defined(CONFIG_FS_RAM)
    returnCode_t test_sd = RAM_DiskWrite(disk, buff, sector, count);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
#endif /* CONFIG_FS_NONE */
}

/**
 * @fn              DiskIoctl(BYTE disk, BYTE cmd, void *buff)
 * @brief           Function that operates a control over disk
 * @param[in]       disk    Disk reference number
 * @param[in]       cmd     Buffer of data to write on disk
 * @param[in,out]   buff    Buffer to send/receive control data
 * @retval          RES_PARERR if disk is not DISK0_REF or count is null
 * @retval          RES_NOTRDY if disk is not ready
 * @retval          RES_ERROR if IO control has encountered an error
 * @retval          RES_OK else
 */
DRESULT DiskIoctl(BYTE disk, BYTE cmd, void *buff)
{
#if defined(CONFIG_FS_NONE)
    (void)(disk);
    (void)(cmd);
    (void)(buff);
    return RES_OK;
#else
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
#if defined(CONFIG_FS_SD)
    returnCode_t test_sd = SD_DiskIoctl(disk, cmd, buff);
#elif defined(CONFIG_FS_SPISD)
    returnCode_t test_sd = SpiSD_DiskIoctl(disk, cmd, buff);
#elif defined(CONFIG_FS_RAM)
    returnCode_t test_sd = RAM_DiskIoctl(disk, cmd, buff);
#else
#error Please #define CONFIG_FS_SD, CONFIG_FS_SPISD, CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    if (test_sd != RET_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
#endif /* CONFIG_FS_NONE */
}
