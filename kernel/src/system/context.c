/**
 * @file    context.c
 * @author  Théo Bessel
 * @brief   Source file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/context.h"
#include "system/error_report.h"
#include "system/sysinfo.h"
#include "drv/memories.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

returnCode_t ReadContext(context_t *context);
returnCode_t WriteContext(context_t context);

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
    if (ReadContext(&context) != RET_SUCCESSFUL)
    {
        KernelPanic();
    }
    else
    {
        // If the boot count and failed boot count are not defined, set them to 0
        // if (context.boot == 0xffu || context.failedBoot == 0xffu)
        // {
        //     context.boot       = 0u;
        //     context.failedBoot = 0u;
        // }

        // If the state is not defined, set it to nominal
        // if (context.state == 0xffu)
        // {
        //     context.state = SOFTWARE_STATE_NOMINAL;
        // }

        // Increment the boot count depending on the state
        // if (context.state == SOFTWARE_STATE_NOMINAL)
        // {
        //     context.boot += 4u;
        // }
        // else
        // {
        //     context.failedBoot++;
        // }

        context.version.major++;
        context.version.minor++;
        context.version.patch++;
        context.boot++;
        context.failedBoot++;

        // If the version is not defined, set it to the current version
        // context.version = g_system_info.version;

        // Write the context
        if (WriteContext(context) != RET_SUCCESSFUL)
        {
            KernelPanic();
        }

        // TEST : Read the context of the kernel
        context_t test_context = { 0 };
        ReadContext(&test_context); // TODO : Issue Here, not getting the edited context

        int a = 0;

        (void) a;
        (void)test_context;
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
    return MemoryRead((uint8_t *)context, 0x0u, sizeof(*context));
}

/**
 * @fn WriteContext(context_t context)
 * @brief Save the context of the kernel using the context memory driver
 * @param[in] context Context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WriteContext(context_t context)
{
    return MemoryWrite((const uint8_t *)&context, 0x0u, sizeof(context_t));
}