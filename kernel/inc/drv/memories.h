/**
 * @file    memories.h
 * @author  Théo Bessel
 * @brief   Header file for memory driver functions
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup memories Memories Drivers
 * @brief Abstraction layer for managing memories.
 * @{
 */

#ifndef MEMORIES_H
#define MEMORIES_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void MemoryInit(void);
extern returnCode_t MemoryRead(uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t MemoryWrite(uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t MemoryErase(uint32_t addr, uint32_t len);

#endif /* MEMORIES_H */

/**
 * @}
 * @}
 * @}
 */