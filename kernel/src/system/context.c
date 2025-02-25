/**
 * @file    context.c
 * @author  Théo Bessel
 * @brief   Source file for Context API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/context.h"
#include "drv/drv_ctxmem.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

returnCode_t ReadContext(context_t *context);
returnCode_t WriteContext(context_t context);

/*********************** Getters and Setters Declarations ********************/

returnCode_t ReadContextSoftwareVersion(softwareVersion_t *version);
returnCode_t WriteContextSoftwareVersion(softwareVersion_t version);
returnCode_t ReadContextSoftwareState(softwareState_t *state);
returnCode_t WriteContextSoftwareState(softwareState_t state);
returnCode_t ReadContextBootCount(bootCount_t *boot);
returnCode_t WriteContextBootCount(bootCount_t boot);
returnCode_t ReadContextFailedBootCount(bootCount_t *failedBoot);
returnCode_t WriteContextFailedBootCount(bootCount_t failedBoot);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn ReadContext(context_t *context)
 * @brief Read the context of the kernel using the context memory driver
 * @param[out] context Pointer to the context structure
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReadContext(context_t *context)
{
    return CtxMemRead((uint8_t *)context, 0x0u, 0x0u, sizeof(*context));
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
    return CtxMemWrite((const uint8_t *)&context, 0x0u, 0x0u, sizeof(context_t));
}

/*********************** Getters and Setters Definitions *********************/

/**
 * @fn ReadContextSoftwareVersion(softwareVersion_t* version)
 * @brief Get the software version from the context
 * @param[out] version Pointer to the software version
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t ReadContextSoftwareVersion(softwareVersion_t *version)
{
    return CtxMemRead((uint8_t *)version, 0x0u, offsetof(context_t, version), sizeof(*version));
}

/**
 * @fn WriteContextSoftwareVersion(softwareVersion_t version)
 * @brief Update the software version in the context
 * @param[in] version Software version
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t WriteContextSoftwareVersion(softwareVersion_t version)
{
    return CtxMemWrite((const uint8_t *)(&version), 0x0u, offsetof(context_t, version), sizeof(version));
}

/**
 * @fn ReadContextSoftwareState(softwareState_t* state)
 * @brief Get the software state from the context
 * @param[out] state Pointer to the software state
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t ReadContextSoftwareState(softwareState_t *state)
{
    return CtxMemRead((uint8_t *)state, 0x0u, offsetof(context_t, state), sizeof(*state));
}

/**
 * @fn WriteContextSoftwareState(softwareState_t state)
 * @brief Update the software state in the context
 * @param[in] state Software state
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t WriteContextSoftwareState(softwareState_t state)
{
    return CtxMemWrite((const uint8_t *)(&state), 0x0u, offsetof(context_t, state), sizeof(state));
}

/**
 * @fn ReadContextBootCount(bootCount_t* boot)
 * @brief Get the boot count from the context
 * @param[out] boot Pointer to the boot count
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t ReadContextBootCount(bootCount_t *boot)
{
    return CtxMemRead((uint8_t *)boot, 0x0u, offsetof(context_t, boot), sizeof(*boot));
}

/**
 * @fn WriteContextBootCount(bootCount_t boot)
 * @brief Update the boot count in the context
 * @param[in] boot Boot count
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t WriteContextBootCount(bootCount_t boot)
{
    return CtxMemWrite((const uint8_t *)(&boot), 0x0u, offsetof(context_t, boot), sizeof(boot));
}

/**
 * @fn ReadContextFailedBootCount(bootCount_t* failedBoot)
 * @brief Get the failed boot count from the context
 * @param[out] failedBoot Pointer to the failed boot count
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t ReadContextFailedBootCount(bootCount_t *failedBoot)
{
    return CtxMemRead((uint8_t *)failedBoot, 0x0u, offsetof(context_t, failedBoot), sizeof(*failedBoot));
}

/**
 * @fn WriteContextFailedBootCount(bootCount_t failedBoot)
 * @brief Update the failed boot count in the context
 * @param[in] failedBoot Failed boot count
 * @retval      #RET_INVALID_PARAM if an error occurs in the context memory driver
 * @retval      #RET_SUCCESSFUL else
 */
ATTR_INLINE returnCode_t WriteContextFailedBootCount(bootCount_t failedBoot)
{
    return CtxMemWrite((const uint8_t *)(&failedBoot), 0x0u, offsetof(context_t, failedBoot), sizeof(failedBoot));
}