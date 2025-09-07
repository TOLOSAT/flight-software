/**
 * @file    drv_ram.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for RAM-emulated memory
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>
#include "drv/memories.h"
#include "drv/memories/drv_ram.h"
#include "core/irq.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define RAM_MEM_SECTOR_SIZE           512u                            /**< Sector size in bytes */
#define RAM_MEM_BLOCK_SIZE_IN_SECTORS 1u                              /**< Erase block size in sectors (fixed to 1 for RAM) */
#define RAM_MEM_BASE_ADDR(ram_inst)   ((ram_inst)->p_conf->base_addr) /**< Macro to get RAM base address */
#define RAM_MEM_SIZE(ram_inst)        ((ram_inst)->p_conf->size)      /**< Macro to get RAM size */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              RamOpen(ramInst_t *ram_inst, const ramConf_t *const ram_conf)
 * @brief           Function that initialise a RAM memory
 * @param[in,out]   ram_inst   Instance that contains RAM handlers
 * @param[in]       ram_conf   Configuration that contains RAM parameters
 * @retval          #RET_SUCCESSFUL if creation succeed
 * @retval          #RET_INVALID_PARAM if a pointer is null
 */
returnCode_t RamOpen(ramInst_t *ram_inst, const ramConf_t *const ram_conf)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ram_inst != NULL) && (ram_conf != NULL))
    {
        // Check if base address and size are sector aligned
        if ((((uintptr_t)ram_conf->base_addr & (RAM_MEM_SECTOR_SIZE - 1u)) == 0u) && //
            ((uintptr_t)ram_conf->base_addr != 0u) &&                                //
            ((ram_conf->size % RAM_MEM_SECTOR_SIZE) == 0u) &&                        //
            (ram_conf->size != 0u))                                                  //
        {
            // Finaly link the conf pointer
            ram_inst->p_conf = ram_conf;
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
}

/**
 * @fn          RamWrite(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that writes onto a RAM memory
 * @param[in]   ram_inst    Instance that contains RAM parameters and RAM Handler
 * @param[in]   sector      Sector number from which data will be written
 * @param[in]   data        Pointer from which data will be copied
 * @param[in]   length      Number of sectors that will be written
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null or parameters are out of range
 */
returnCode_t RamWrite(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ram_inst != NULL) && (length != 0u) && (data != NULL))
    {
        uint32_t sector_count = RAM_MEM_SIZE(ram_inst) / RAM_MEM_SECTOR_SIZE;

        // Check bounds
        if (((uint32_t)sector < sector_count) && (((uint32_t)sector + length) <= sector_count))
        {
            // Compute target address
            uint8_t *base = ram_inst->p_conf->base_addr;
            uint8_t *dst  = &base[sector * RAM_MEM_SECTOR_SIZE];

            // Copy data into RAM
            (void)memcpy(dst, data, (size_t)length * RAM_MEM_SECTOR_SIZE);
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
 * @fn          RamRead(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
 * @brief       Function that read onto an RAM memory
 * @param[in]   ram_inst    Instance that contains RAM parameters and RAM Handler
 * @param[in]   sector      Sector numero from wich data will be read
 * @param[out]  data        Pointer to where data will be copied
 * @param[in]   length      Number of block that will be read
 * @retval      #RET_SUCCESSFUL if data has been written successfully
 * @retval      #RET_INVALID_PARAM if one pointer is null or parameters are out of range
 */
returnCode_t RamRead(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((ram_inst != NULL) && (length != 0u) && (data != NULL))
    {
        uint32_t sector_count = RAM_MEM_SIZE(ram_inst) / RAM_MEM_SECTOR_SIZE;

        // Check bounds
        if (((uint32_t)sector < sector_count) && (((uint32_t)sector + length) <= sector_count))
        {
            // Compute source address
            const uint8_t *base = ram_inst->p_conf->base_addr;
            const uint8_t *src  = &base[sector * RAM_MEM_SECTOR_SIZE];

            // Copy data from RAM
            (void)memcpy(data, src, (size_t)length * RAM_MEM_SECTOR_SIZE);
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
 * @fn              RamIoctl(ramInst_t *ram_inst, uint32_t cmd, void *data, uint32_t data_size);
 * @brief           Function that adds advanced control to the driver
 * @param[in,out]   ram_inst    Instance that contains RAM handlers
 * @param[in]       cmd         IO Control command
 * @param[in,out]   data        IO Control command
 * @param[in]       data_size   IO Control data size
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 * @retval          #RET_NOT_AVAILABLE if action cannot be performed because driver is busy
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t RamIoctl(ramInst_t *ram_inst, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (ram_inst != NULL)
    {
        switch (cmd)
        {
            case IOCTL_MEMORY_GET_STATUS :
                if (data_size == sizeof(memoryStatus_t))
                {
                    *(memoryStatus_t *)data = MEMORY_READY;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_SYNC :
                // Sync is not required for this RAM memory, so do nothing
                break;
            case IOCTL_MEMORY_GET_SECTOR_COUNT :
                if (data_size == sizeof(memorySectorCount_t))
                {
                    *(uint32_t *)data = RAM_MEM_SIZE(ram_inst) / RAM_MEM_SECTOR_SIZE;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_SECTOR_SIZE :
                if (data_size == sizeof(memorySectorSize_t))
                {
                    *(memorySectorSize_t *)data = RAM_MEM_SECTOR_SIZE;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_GET_BLOCK_SIZE :
                if (data_size == sizeof(memoryBlockSize_t))
                {
                    *(memoryBlockSize_t *)data = RAM_MEM_BLOCK_SIZE_IN_SECTORS;
                }
                else
                {
                    return_value = RET_INVALID_PARAM;
                }
                break;
            case IOCTL_MEMORY_ERASE_BLOCK :
                // TO DO : to be implemented
                return_value = RET_INVALID_PARAM;
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
}

/**
 * @fn              RamClose(ramInst_t *ram_inst)
 * @brief           Function that desinit the RAM connection
 * @param[in,out]   ram_inst   Instance that contains RAM handlers
 * @retval          #RET_SUCCESSFUL if changing parameters succeed
 * @retval          #RET_INVALID_PARAM if instance is a null pointer
 */
returnCode_t RamClose(ramInst_t *ram_inst)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (ram_inst != NULL)
    {
        // Nothing to do
        (void)(ram_inst);
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}
