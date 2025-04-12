/**
 * @file    memdrv_qspi.h
 * @author  Théo Bessel
 * @brief   Header for QSPI Memory driver
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup memdrv_qspi QSPI Memory Driver
 * @brief Abstraction layer for controlling a NAND QSPI memory.
 * @{
 */

#ifndef MEMDRV_QSPI_H
#define MEMDRV_QSPI_H

/******************************* Include Files *******************************/

#include "boot_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t QSPI_MemoryInit(void);
extern returnCode_t QSPI_MemoryRead(uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t QSPI_MemoryWrite(uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t QSPI_MemoryErase(uint32_t addr, uint32_t len);

#endif /* MEMDRV_QSPI_H */

/**
 * @}
 * @}
 * @}
 */
