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
 * @fn          DiskInitialize(BYTE disk)
 * @brief       Function that initialise disk drive
 * @param[in]   disk    Disk reference number
 * @retval      STA_NOINIT if disk number is not valid
 * @retval      STA_NODISK if disk is not available
 * @retval      0 if disk initialization is a success
 */
DSTATUS DiskInitialize(BYTE disk)
{
    // Variable Initialisation
    DSTATUS res = STA_NOINIT;

    // Function Core
#if defined(CONFIG_FS_RAM)
    returnCode_t test_sd = RAM_DiskInit(disk);
#elif defined(CONFIG_FS_NONE)
    returnCode_t test_sd = RET_SUCCESSFUL;
    (void)(disk);
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    if (test_sd == RET_SUCCESSFUL)
    {
#if defined(CONFIG_FS_RAM)
        res = RAM_DiskStatus(disk);
#elif defined(CONFIG_FS_NONE)
        res = RES_OK;
#else
#error Please #define CONFIG_FS_RAM or CONFIG_FS_NONE
#endif
    }

    return res;
}

/**
 * @fn          DiskStatus(BYTE disk)
 * @brief       Function that returns disk status
 * @param[in]   disk    Driver reference number
 * @return      Disk Status
 */
DSTATUS DiskStatus(BYTE disk)
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
    // Variable Initialisation
    DRESULT res = RES_OK ;

    // Function Core
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
DRESULT DiskWrite(BYTE disk, const BYTE *buff, DWORD sector, UINT count)
{
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
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

    return res;
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
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
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

    return res;
}
