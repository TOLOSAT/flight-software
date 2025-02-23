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