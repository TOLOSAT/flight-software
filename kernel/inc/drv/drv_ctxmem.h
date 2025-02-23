/**
 * @file    drv_ctxmem.h
 * @author  Théo Bessel
 * @brief   Header for Context Memory driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_ctxmem Context Memory Driver
 * @brief Abstraction layer for controlling a context memory.
 * @{
 */

#ifndef DRV_CTXMEM_H
#define DRV_CTXMEM_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len);
extern returnCode_t CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len);

#endif /* DRV_CTXMEM_H */

/**
 * @}
 * @}
 * @}
 */
