/**
 * @file    diskdrv_ram.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for RAM disk driver
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include <string.h>
#include "drv/drv_disk.h"
#include "drv/disk/diskdrv_ram.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define SECTOR_SIZE 512u /**< Size of a sector */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

extern uint32_t __ramfs_start__;
extern uint32_t __ramfs_end__;

static uint32_t *ramfs_ptr = &__ramfs_start__;
static DSTATUS disk_stat   = STA_NOINIT;

/*************************** Functions Definitions ***************************/

/**
 * @fn          RAM_DiskStatus(uint8_t disk)
 * @brief       Function that gets status of the RAM
 * @param[in]   disk    Disk on from which we get the status
 * @return      DSTATUS
 */
DSTATUS RAM_DiskStatus(uint8_t disk)
{
    // Variables Initialization
    DSTATUS return_value = STA_NOINIT;

    // Function Core
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
 * @retval      #RET_INVALID_PARAM if disk does not exist
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t RAM_DiskInit(uint8_t disk)
{
    // Variables Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (disk == DISK0_REF)
    {
        disk_stat &= ~STA_NOINIT;
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
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
    // Variables Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (disk == DISK0_REF)
    {
        (void)memcpy(data, (void *)&ramfs_ptr[addr * SECTOR_SIZE], len * SECTOR_SIZE);
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
    // Variables Initialisation
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if (disk == DISK0_REF)
    {
        (void)memcpy((void *)&ramfs_ptr[addr * SECTOR_SIZE], data, len * SECTOR_SIZE);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              RAM_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
 * @brief           Function that perfoms io control on the RAM disk (get info, change parameters ...)
 * @param[in]       disk    Disk on which we perform the io control
 * @param[in]       cmd     Which can of action is done on the RAM disk
 * @param[in,out]   data    Data shared depending of command
 * @retval          #RET_INVALID_PARAM if the io control is not available for this device
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t RAM_DiskIoctl(uint8_t disk, uint8_t cmd, void *data)
{
    // Variables Initialization
    returnCode_t return_value = RET_SUCCESSFUL;

    // Function Core
    if ((RAM_DiskStatus(disk) & STA_NOINIT) == STA_NOINIT)
    {
        KernelPanic();
    }
    else
    {
        switch (cmd)
        {
            case CTRL_SYNC:
                break;

            case GET_BLOCK_SIZE:
            case GET_SECTOR_SIZE:
                *(WORD *)data = SECTOR_SIZE;
                break;

            case GET_SECTOR_COUNT:
                *(DWORD *)data = ((uint32_t)&__ramfs_end__ - (uint32_t)&__ramfs_start__) / SECTOR_SIZE; // cppcheck-suppress misra-c2012-11.4; Exception: this is the only way to know the section size
                break;

            default:
                KernelPanic();
                break;
        }
    }

    return return_value;
}
