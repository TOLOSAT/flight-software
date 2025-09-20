/**
 * @file    context.c
 * @author  Théo Bessel
 * @brief   Source file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "system/context.h"
#include "system/sysinfo.h"
#include "drv/memories.h"
#include "fdir/fdir.h"

/***************************** Macros Definitions ****************************/

#define ERASED_MEMORY   0xffffffffu /**< Invalid state */
#define MAX_SECTOR_SIZE 4096u       /**< Maximum sector size*/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

#if defined(CONFIG_CONTEXT_ENABLED)
/**
 * @brief Context buffer
 */
static uint8_t context_buff[MAX_SECTOR_SIZE] = { 0 }; // TO DO : to protect correctly
#endif

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitContext(void)
 * @brief       Initialise the context of the kernel
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
void InitContext(void)
{
    // Initialise the flight software context
    context_t context = { 0 };

    // Read the context
    returnCode_t test_context = ReadContext(&context);

    if (test_context != RET_SUCCESSFUL)
    {
        KernelPanic();
    }
    else
    {
        // If the boot count and failed boot count are not defined, set them to 0
        if (context.boot == ERASED_MEMORY)
        {
            context.boot = 0u;
        }
        if (context.critical_error == ERASED_MEMORY)
        {
            context.critical_error = 0u;
        }

        if (context.safe_software_id == (softwareId_t)ERASED_MEMORY)
        {
            context.safe_software_id = 0u;
        }

        if (context.nominal_software_id == (softwareId_t)ERASED_MEMORY)
        {
            context.nominal_software_id = 0u;
        }

        // If the state is not defined, set it to nominal
        if (context.state == (softwareState_t)ERASED_MEMORY)
        {
            context.state = SOFTWARE_STATE_NOMINAL;
        }

        // Set bnco to 0xBB as Big Burgir
        context.bnco = 0xBBu;

        // Increment the boot count
        context.boot++;

        // Set the context version to the current software version
        context.version = g_system_info.version;

        // TODO: Save the debug info

        // Reset the debug info
        context.cfsr       = 0u;
        context.hfsr       = 0u;
        context.registers  = (savedRegisters_t){ 0 };
        context.call_stack = (callStack_t){ 0 };

        // Write the updated context
        test_context = WriteContext(&context);

        if (test_context != RET_SUCCESSFUL)
        {
            KernelPanic();
        }
    }
}

/**
 * @fn          ReadContext(context_t *context)
 * @brief       Read the context of the kernel using the context memory driver
 * @param[out]  context Pointer to the context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReadContext(context_t *context)
{
#if defined(CONFIG_CONTEXT_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (context != NULL)
    {
        memorySectorSize_t sector_size = 0u;

        // Get sector size
        return_value = MemoryIoctl(g_context_mem, IOCTL_MEMORY_GET_SECTOR_SIZE, &sector_size, (length_t)sizeof(memorySectorSize_t));

        if (return_value == RET_SUCCESSFUL)
        {
            if ((sector_size != 0u) && (sector_size <= MAX_SECTOR_SIZE))
            {
                uint8_t *context_bytes = (uint8_t *)context;

                const length_t context_size       = (length_t)sizeof(context_t);
                const memorySector_t sector_count = (memorySector_t)((context_size + (length_t)sector_size - 1u) / (length_t)sector_size);
                memorySector_t sector             = 0u;
                // Iterate over sectors
                while ((sector < sector_count) && (return_value == RET_SUCCESSFUL))
                {
                    uint8_t *dest = &context_bytes[(length_t)sector * (length_t)sector_size];

                    // Compute length to read for this sector (last sector may be partial)
                    const length_t already_read = (length_t)sector * (length_t)sector_size;
                    const length_t remaining    = context_size - already_read;
                    const bool full_sector      = (remaining >= (length_t)sector_size);

                    if (full_sector)
                    {
                        // Direct read into destination buffer (1 sector)
                        return_value = MemoryRead(g_context_mem, sector, dest, (length_t)1u);
                    }
                    else
                    {
                        // Last partial sector: read one full sector into temporary buffer
                        return_value = MemoryRead(g_context_mem, sector, context_buff, (length_t)1u);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // Copy only valid bytes
                            (void)memcpy(dest, context_buff, remaining);
                        }
                    }

                    ++sector;
                }
            }
            else
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
#else
    (void)(context);
    return RET_SUCCESSFUL;
#endif
}

/**
 * @fn          WriteContext(context_t *context)
 * @brief       Save the context of the kernel using the context memory driver
 * @param[in]   context Context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WriteContext(context_t *context)
{
#if defined(CONFIG_CONTEXT_ENABLED)
    returnCode_t return_value = RET_SUCCESSFUL;

    // Check parameter(s)
    if (context != NULL)
    {
        memorySectorSize_t sector_size = 0u;

        // Get sector size
        return_value = MemoryIoctl(g_context_mem, IOCTL_MEMORY_GET_SECTOR_SIZE, &sector_size, (length_t)sizeof(memorySectorSize_t));

        if (return_value == RET_SUCCESSFUL)
        {
            if ((sector_size != 0u) && (sector_size <= MAX_SECTOR_SIZE))
            {
                uint8_t *context_bytes = (uint8_t *)context;

                const length_t context_size       = (length_t)sizeof(context_t);
                const memorySector_t sector_count = (memorySector_t)((context_size + (length_t)sector_size - 1u) / (length_t)sector_size);
                memorySector_t sector             = 0u;
                // Iterate over sectors
                while ((sector < sector_count) && (return_value == RET_SUCCESSFUL))
                {
                    uint8_t *src = &context_bytes[(length_t)sector * (length_t)sector_size];

                    // Compute length to write for this sector (last sector may be partial)
                    const length_t already_written = (length_t)sector * (length_t)sector_size;
                    const length_t remaining       = context_size - already_written;
                    const bool full_sector         = (remaining >= (length_t)sector_size);

                    if (full_sector)
                    {
                        // Full sector: write directly from source buffer, length in sectors = 1
                        return_value = MemoryWrite(g_context_mem, sector, src, (length_t)1u);
                    }
                    else
                    {
                        // Partial last sector:
                        // 1) Read the existing sector to avoid corrupting bytes beyond 'remaining'
                        return_value = MemoryRead(g_context_mem, sector, context_buff, (length_t)1u);
                        if (return_value == RET_SUCCESSFUL)
                        {
                            // 2) Overwrite only the valid bytes in the temporary buffer
                            (void)memcpy(context_buff, src, remaining);

                            // 3) Write back one full sector
                            return_value = MemoryWrite(g_context_mem, sector, context_buff, (length_t)1u);
                        }
                    }

                    ++sector;
                }
            }
            else
            {
                KernelPanic();
            }
        }
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    return return_value;
#else
    (void)(context);
    return RET_SUCCESSFUL;
#endif
}

/**
 * @fn          EraseContext(void)
 * @brief       Erase the context of the kernel using the context memory driver
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t EraseContext(void)
{
    context_t empty_context = { 0 };

    // Set bnco to 0xBBu as Big Burgir
    empty_context.bnco = 0xBBu;

    return WriteContext(&empty_context);
}