/**
 * @file    tolosat_hal_fs.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static DSTATUS DiskInitialize (BYTE pdrv);
static DSTATUS DiskStatus (BYTE pdrv);
static DRESULT DiskRead (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskWrite (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
static DRESULT DiskIoctl (BYTE pdrv, BYTE cmd, void *buff);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              FsOpen(FsInst_t *fs_inst)
 * @brief           Function that initialise a FS
 * @param[in,out]   fs_inst Instance that contains FS parameters and driver
 * @retval          #FCT_INVALID_PARAM if fs_inst is null pointer
 * @retval          #FCT_ERROR if cannot create FS
 * @retval          #FCT_SUCCESSFUL else
 */
halStatus_t FsOpen(FsInst_t *fs_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if (fs_inst != NULL)
    {
        fs_inst->driver.disk_initialize = DiskInitialize;
        fs_inst->driver.disk_status = DiskStatus;
        fs_inst->driver.disk_read = DiskRead;
        fs_inst->driver.disk_write = DiskWrite;
        fs_inst->driver.disk_ioctl = DiskIoctl;

        uint8_t test_val = FATFS_LinkDriver(&fs_inst->driver, fs_inst->disk_path);
        if(test_val != 0u)
        {
            return_value = FCT_ERROR;
        }
    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return return_value;
}

static DSTATUS DiskInitialize (BYTE pdrv)
{
    (void)(pdrv);

    return STA_NOINIT;
}

static DSTATUS DiskStatus (BYTE pdrv)
{
    (void)(pdrv);

    return STA_NOINIT;
}

static DRESULT DiskRead (BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    (void)(pdrv);
    (void)(buff);
    (void)(sector);
    (void)(count);

    return RES_OK;
}

static DRESULT DiskWrite (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    (void)(pdrv);
    (void)(buff);
    (void)(sector);
    (void)(count);

    return RES_OK;
}

static DRESULT DiskIoctl (BYTE pdrv, BYTE cmd, void *buff)
{
    (void)(pdrv);
    (void)(cmd);
    (void)(buff);

    return RES_ERROR;
}