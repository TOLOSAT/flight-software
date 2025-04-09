/**
 * @file    context.c
 * @author  Théo Bessel
 * @brief   Source file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/context.h"
#include "system/sysinfo.h"
#include "drv/memories.h"
#include "fdir/fdir.h"

#include <string.h>

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

returnCode_t ReadContext(context_t *context);
returnCode_t WriteContext(context_t *context);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn InitContext(void)
 * @brief Initialise the context of the kernel
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
        if (context.boot == 0xffffffffu)
        {
            context.boot = 0u;
        }
        if (context.failedBoot == 0xffffffffu)
        {
            context.failedBoot = 0u;
        }

        // If the state is not defined, set it to nominal
        if (context.state == (softwareState_t)0xffffffffu)
        {
            context.state = SOFTWARE_STATE_NOMINAL;
        }

        // Increment the boot count depending on the state
        if (context.state == SOFTWARE_STATE_NOMINAL)
        {
            context.boot++;
        }
        else
        {
            context.failedBoot++;
        }

        // Set the context version to the current software version
        context.version = g_system_info.version;

        // Write the updated context
        test_context = WriteContext(&context);

        if (test_context != RET_SUCCESSFUL)
        {
            KernelPanic();
        }

        __NOP();
    }
}

/**
 * @fn ReadContext(context_t *context)
 * @brief Read the context of the kernel using the context memory driver
 * @param[out] context Pointer to the context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReadContext(context_t *context)
{
    returnCode_t test_context   = RET_SUCCESSFUL;
    uint8_t context_array[256u] = { 0 };

    test_context = MemoryRead(context_array, 0x0u, 256u);

    if (test_context == RET_SUCCESSFUL)
    {
        if (sizeof(context_t) > sizeof(context_array))
        {
            test_context = RET_INVALID_PARAM;
        }
        else
        {
            memcpy((uint8_t *)context, context_array, sizeof(context_t));
        }
    }

    return test_context;
}

/**
 * @fn WriteContext(context_t *context)
 * @brief Save the context of the kernel using the context memory driver
 * @param[in] context Context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WriteContext(context_t *context)
{
    returnCode_t test_context   = RET_SUCCESSFUL;
    uint8_t context_array[256u] = { 0 };

    if (sizeof(context_t) > sizeof(context_array))
    {
        test_context = RET_INVALID_PARAM;
    }
    else
    {
        memcpy(context_array, (uint8_t *)context, sizeof(context_t));
        test_context = MemoryWrite(context_array, 0x0u, 256u);
    }

    return test_context;
}