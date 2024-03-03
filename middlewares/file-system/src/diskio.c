/**
 * @file    diskio.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT Disk IO functions
 * @date    17/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "diskio.h"

#if defined(FS_SDMMC)
#include "sdmmc_driver.h"
#elif defined(FS_SPI)
#include "spisd_driver.h"
#else
#error Please #define FS_SDMMC or FS_SPI
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
    // Variable Initialisation
    DSTATUS res = STA_NOINIT;

    // Function Core
#if defined(FS_SDMMC)
    fsStatus_t test_sd = SD_Init(disk);
#elif defined(FS_SPI)
    fsStatus_t test_sd = SpiSD_Init(disk);
#else
#error Please #define FS_SDMMC or FS_SPI
#endif
    if (test_sd == FS_SUCCESSFUL)
    {
#if defined(FS_SDMMC)
        res = SD_GetStatus(disk);
#elif defined(FS_SPI)
        res = SpiSD_GetStatus(disk);
#else
#error Please #define FS_SDMMC or FS_SPI
#endif
    }

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
#if defined(FS_SDMMC)
    return SD_GetStatus(disk);
#elif defined(FS_SPI)
    return SpiSD_GetStatus(disk);
#else
#error Please #define FS_SDMMC or FS_SPI
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
    // Variable Initialisation
    DRESULT res = RES_OK ;

    // Function Core
#if defined(FS_SDMMC)
    fsStatus_t test_sd = SD_ReadBlocks(disk, buff, sector, count);
#elif defined(FS_SPI)
    fsStatus_t test_sd = SpiSD_ReadBlocks(disk, buff, sector, count);
#else
#error Please #define FS_SDMMC or FS_SPI
#endif
    if (test_sd != FS_SUCCESSFUL)
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
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
#if defined(FS_SDMMC)
    fsStatus_t test_sd = SD_WriteBlocks(disk, buff, sector, count);
#elif defined(FS_SPI)
    fsStatus_t test_sd = SpiSD_WriteBlocks(disk, buff, sector, count);
#else
#error Please #define FS_SDMMC or FS_SPI
#endif
    if (test_sd != FS_SUCCESSFUL)
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
    // Variable Initialisation
    DRESULT res = RES_OK;

    // Function Core
#if defined(FS_SDMMC)
    fsStatus_t test_sd = SD_Ioctl(disk, cmd, buff);
#elif defined(FS_SPI)
    fsStatus_t test_sd = SpiSD_Ioctl(disk, cmd, buff);
#else
#error Please #define FS_SDMMC or FS_SPI
#endif
    if (test_sd != FS_SUCCESSFUL)
    {
        res = RES_ERROR;
    }

    return res;
}
