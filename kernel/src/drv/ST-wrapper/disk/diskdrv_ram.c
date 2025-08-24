/**
 * @file    diskdrv_ram.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for RAM disk driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>
#include "drv/disks.h"
#include "drv/disk/diskdrv_ram.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define RAM_SECTOR_SIZE           512u /**< Sector size in bytes */
#define RAM_BLOCK_SIZE_IN_SECTORS 1u   /**< Erase block size in sectors (fixed to 1 for RAM) */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern uint8_t __ramfs_start__;
extern uint8_t __ramfs_end__;

static diskStatus_t disk_stat = STA_NOINIT;
static uint32_t disk_size     = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn          RAM_DiskStatus(uint8_t disk)
 * @brief       Function that gets status of the RAM
 * @param[in]   disk    Disk from which we get the status
 * @return      diskStatus_t
 */
diskStatus_t RAM_DiskStatus(uint8_t disk)
{
    diskStatus_t return_value = STA_NOINIT;

    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        return_value = disk_stat;
    }
    else
    {
        return_value = STA_NODISK;
    }

    return return_value;
}

/**
 * @fn          RAM_DiskInit(uint8_t disk)
 * @brief       Function that initialises an RAM disk
 * @param[in]   disk    Disk that will be initialised
 * @retval      #STA_NODISK if disk does not exist
 * @retval      #0 else
 */
diskStatus_t RAM_DiskInit(uint8_t disk)
{
    // Check parameter(s)
    if (disk == DISK0_REF)
    {
        disk_size = (uint32_t)&__ramfs_end__ - (uint32_t)&__ramfs_start__; // cppcheck-suppress misra-c2012-11.4; Exception: this is the only way to
                                                                           // know the section size
        disk_stat &= ~STA_NOINIT;
    }
    else
    {
        disk_stat = STA_NODISK;
    }

    return disk_stat;
}

/**
 * @fn          RAM_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that reads RAM disk blocks
 * @param[in]   disk    Disk that is read
 * @param[out]  data    Pointer to the data that will be read
 * @param[in]   addr    Address of the data that will be read
 * @param[in]   len     Number of block that will be read
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_NOT_AVAILABLE if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t RAM_DiskRead(uint8_t disk, uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        uint8_t *start = &((uint8_t *)&__ramfs_start__)[addr * RAM_SECTOR_SIZE]; // cppcheck-suppress objectIndex; This is the desired behavior
        uint32_t size  = len * RAM_SECTOR_SIZE;
        if (&start[len] <= &__ramfs_end__) // cppcheck-suppress [objectIndex, comparePointers]; This is the desired behavior
        {
            (void)memcpy(data, start, size);
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RAM_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
 * @brief       Function that writes RAM disk
 * @param[in]   disk    Disk that is written
 * @param[in]   data    Pointer to the data that will be written
 * @param[in]   addr    Address of the data that will be written
 * @param[in]   len     Number of block that will be written
 * @retval      #RET_INVALID_PARAM if disk does not exist, len equal zero, pointer is null
 * @retval      #RET_NOT_AVAILABLE if disk is not available
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t RAM_DiskWrite(uint8_t disk, const uint8_t *data, uint32_t addr, uint32_t len)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((disk == DISK0_REF) && (len != 0u) && (data != NULL))
    {
        uint8_t *start = &((uint8_t *)&__ramfs_start__)[addr * RAM_SECTOR_SIZE]; // cppcheck-suppress objectIndex; This is the desired behavior
        uint32_t size  = len * RAM_SECTOR_SIZE;
        if (&start[len] <= &__ramfs_end__) // cppcheck-suppress [objectIndex, comparePointers]; This is the desired behavior
        {
            (void)memcpy(start, data, size);
        }
        else
        {
            return_value = RET_INVALID_PARAM;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              RAM_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that performs io control on the RAM disk (get info, change parameters ...)
 * @param[in]       disk    Disk on which we perform the io control
 * @param[in]       cmd     Which can of action is done on the RAM disk
 * @param[in,out]   data    Data shared depending of command
 * @retval          #RET_INVALID_PARAM if the io control is not available for this device
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t RAM_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check disk status
    if ((RAM_DiskStatus(disk) & STA_NOINIT) == STA_NOINIT)
    {
        KernelPanic();
    }
    else
    {
        switch (cmd)
        {
            case CTRL_SYNC :
                break;

            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT :
                *(uint32_t *)data = disk_size / RAM_SECTOR_SIZE;
                break;

            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE :
                *(uint16_t *)data = RAM_SECTOR_SIZE;
                break;

            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE :
                *(uint32_t *)data = RAM_BLOCK_SIZE_IN_SECTORS;
                break;

            default :
                return_value = RET_INVALID_PARAM;
                break;
        }
    }

    return return_value;
}
