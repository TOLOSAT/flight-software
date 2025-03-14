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

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void QspiNandInit(void);
extern returnCode_t CtxMemRead(uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len);
extern returnCode_t CtxMemWrite(const uint8_t *data, uint32_t addr, uint32_t offset, uint32_t len);

#endif /* MEMDRV_QSPI_H */

/**
 * @}
 * @}
 * @}
 */
