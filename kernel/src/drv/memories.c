/**
 * @file    memories.c
 * @author  Théo Bessel
 * @author  Merlin Kooshmanian
 * @brief   Source file for memory driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/memories.h"
#include "fdir/fdir.h"

#if defined(CONFIG_HAS_RAM_MEMORY)
// #include "memdrv_ram.h"
#endif
#if defined(CONFIG_HAS_SD_MEMORY)
// #include "memdrv_sd.h"
#endif
#if defined(CONFIG_HAS_SPISD_MEMORY)
// #include "memdrv_spisd.h"
#endif
#if defined(CONFIG_HAS_QSPI_FLASH_MEMORY)
// #include "memdrv_qspi.h"
#endif
#if defined(CONFIG_HAS_FMC_NAND_MEMORY)
// #include "memdrv_nand.h"
#endif

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      InitMemories(void)
 * @brief   Function that initialises the memories
 * @return  Nothing
 */
void InitMemories(void)
{
    // returnCode_t return_value;
    memoryNo_t memory = 1u;

    // Init all memories
    while (MEMORY_CONF(memory).memory != NO_MEMORY)
    {
        // Initialise memory depending of the memory type
        switch (MEMORY_CONF(memory).type)
        {
            case MEMORY_RAM :
                /* To Do */
                break;
            case MEMORY_SD :
                /* To Do */
                break;
            case MEMORY_SPISD :
                /* To Do */
                break;
            case MEMORY_QSPIFLASH :
                /* To Do */
                break;
            case MEMORY_NAND :
                /* To Do */
                break;
            default :
                KernelPanic();
                break;
        }

        // Check memory init return
        // if (return_value != RET_SUCCESSFUL)
        // {
        //     KernelPanic();
        // }

        // Indicates the memory is initialised
        MEMORY_DESC(memory).status = DESC_USED;
        memory++;
    }
}

/**
 * @fn          MemoryWrite(memoryNo_t memory, uint32_t sector, data_t data, length_t length)
 * @brief       Function that writes data to a memory
 * @param[in]   memory  Memory numero
 * @param[in]   sector  Base sector from which the writing starts
 * @param[in]   data    Data that will be sent to the device
 * @param[in]   length  Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or memory is not valid
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t MemoryWrite(memoryNo_t memory, uint32_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_MEMORY(memory)) && (data != NULL))
    {
        // Then get memory and type
        memoryType_t type = MEMORY_CONF(memory).type;

        // Then use the correct driver to write
        switch (type)
        {
            case MEMORY_RAM :
                /* To Do */
                (void)(sector);
                (void)(data);
                (void)(length);
                break;
            case MEMORY_SD :
                /* To Do */
                break;
            case MEMORY_SPISD :
                /* To Do */
                break;
            case MEMORY_QSPIFLASH :
                /* To Do */
                break;
            case MEMORY_NAND :
                /* To Do */
                break;
            default :
                KernelPanic();
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
 * @fn          MemoryRead(memoryNo_t memory, uint32_t sector, data_t data, length_t length)
 * @brief       Function that reads data from a memory
 * @param[in]   memory  Memory numero
 * @param[in]   sector  Base sector from which the reading starts
 * @param[out]  data    Data that will be received to the memory
 * @param[in]   length  Length of the data
 * @retval      #RET_INVALID_PARAM if data is a null pointer or memory is not valid
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t MemoryRead(memoryNo_t memory, uint32_t sector, data_t data, length_t length)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if ((IS_A_VALID_MEMORY(memory)) && (data != NULL))
    {
        // Then get memory and type
        memoryType_t type = MEMORY_CONF(memory).type;

        // Then use the correct driver to read
        switch (type)
        {
            case MEMORY_RAM :
                /* To Do */
                (void)(sector);
                (void)(data);
                (void)(length);
                break;
            case MEMORY_SD :
                /* To Do */
                break;
            case MEMORY_SPISD :
                /* To Do */
                break;
            case MEMORY_QSPIFLASH :
                /* To Do */
                break;
            case MEMORY_NAND :
                /* To Do */
                break;
            default :
                KernelPanic();
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
 * @fn              MemoryIoctl(memoryNo_t memory, uint32_t cmd, void *data, uint32_t data_size)
 * @brief           Function that allows specific control over the memory
 * @param[in]       memory      Memory numero
 * @param[in]       cmd         IO control command
 * @param[in,out]   data        Data related to the command (if any), can be input or output
 * @param[in]       data_size   Data length (if any)
 * @retval          #RET_INVALID_PARAM if memory is not valid
 * @retval          #RET_SUCCESSFUL else
 */
returnCode_t MemoryIoctl(memoryNo_t memory, uint32_t cmd, void *data, uint32_t data_size)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (IS_A_VALID_MEMORY(memory))
    {
        // First get memory and type
        memoryType_t type = MEMORY_CONF(memory).type;

        // Memory specific IOCTL
        switch (type)
        {
            case MEMORY_RAM :
                /* To Do */
                (void)(cmd);
                (void)(data);
                (void)(data_size);
                break;
            case MEMORY_SD :
                /* To Do */
                break;
            case MEMORY_SPISD :
                /* To Do */
                break;
            case MEMORY_QSPIFLASH :
                /* To Do */
                break;
            case MEMORY_NAND :
                /* To Do */
                break;
            default :
                KernelPanic();
                break;
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
}