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

void SaveContext(context_t *context);
context_t ReadContext(void);

/*********************** Getters and Setters Declarations ********************/

softwareVersion_t GetContextSoftwareVersion(void);
softwareState_t GetContextSoftwareState(void);
bootCount_t GetContextBootCount(void);
void SetContextSoftwareVersion(softwareVersion_t version);
void SetContextSoftwareState(softwareState_t state);
void SetContextBootCount(bootCount_t boot);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn SaveContext(context_t *context)
 * @brief Save the context of the kernel using the context memory driver
 * @param[in] context Pointer to the context structure
 * @return Nothing
 */
void SaveContext(context_t *context)
{
    CtxMemWrite((const uint8_t *)context, 0x0u, 0x0u, sizeof(context_t));
}

/**
 * @fn RestoreContext(void)
 * @brief Restore the context of the kernel using the context memory driver
 * @return Pointer to the context structure
 */
context_t ReadContext(void)
{
    context_t context = { 0 };
    CtxMemRead((uint8_t *)&context, 0x0u, 0x0u, sizeof(context_t));
    return context;
}

/*********************** Getters and Setters Definitions *********************/

/**
 * @fn GetContextSoftwareVersion(void)
 * @brief Get the software version from the context
 * @return Software version
 */
ATTR_INLINE softwareVersion_t GetContextSoftwareVersion(void)
{
    softwareVersion_t version = { 0 };
    CtxMemRead((uint8_t *)&version, 0x0u, offsetof(context_t, version), sizeof(version));
    return version;
}

/**
 * @fn GetContextSoftwareState(void)
 * @brief Get the software state from the context
 * @return Software state
 */
ATTR_INLINE softwareState_t GetContextSoftwareState(void)
{
    softwareState_t state = SOFTWARE_STATE_ERROR;
    CtxMemRead((uint8_t *)&state, 0x0u, offsetof(context_t, state), sizeof(state));
    return state;
}

/**
 * @fn GetContextBootCount(void)
 * @brief Get the boot count from the context
 * @return Boot count
 */
ATTR_INLINE bootCount_t GetContextBootCount(void)
{
    bootCount_t boot = 0;
    CtxMemRead((uint8_t *)&boot, 0x0u, offsetof(context_t, boot), sizeof(boot));
    return boot;
}

/**
 * @fn UpdateContextSoftwareVersion(softwareVersion_t version)
 * @brief Update the software version in the context
 * @param[in] version Software version
 * @return Nothing
 */
ATTR_INLINE void SetContextSoftwareVersion(softwareVersion_t version)
{
    CtxMemWrite((const uint8_t *)(&version), 0x0u, offsetof(context_t, version), sizeof(version));
}

/**
 * @fn UpdateContextSoftwareState(softwareState_t state)
 * @brief Update the software state in the context
 * @param[in] state Software state
 * @return Nothing
 */
ATTR_INLINE void SetContextSoftwareState(softwareState_t state)
{
    CtxMemWrite((const uint8_t *)(&state), 0x0u, offsetof(context_t, state), sizeof(state));
}

/**
 * @fn UpdateContextBootCount(bootCount_t boot)
 * @brief Update the boot count in the context
 * @param[in] boot Boot count
 * @return Nothing
 */
ATTR_INLINE void SetContextBootCount(bootCount_t boot)
{
    CtxMemWrite((const uint8_t *)(&boot), 0x0u, offsetof(context_t, boot), sizeof(boot));
}