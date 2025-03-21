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
#include "ff_gen_drv.h"

/***************************** Macros Definitions ****************************/

// QUADSPI defines

#define QSPI_READ_CMD                0xebu /**< Quad Output Fast Read command */
#define QSPI_WRITE_CMD               0x32u /**< Quad Input Fast Program command */
#define QSPI_ERASE_CMD               0x20u /**< Sector Erase command */

#define QSPI_DUMMY_CLOCK_CYCLES_READ 6u  /**< Number of dummy cycles for Quad Read */
#define QSPI_CLOCK_PRESCALER         4u  /**< Clock prescaler */
#define QSPI_FIFO_THRESHOLD          1u  /**< FIFO threshold */
#define QSPI_FLASH_SIZE              22u /**< Flash size */

#define QSPI_REGISTER_READ_OFFSET    5u /**< Register select read offset */
#define QSPI_REGISTER_WRITE_OFFSET   1u /**< Register select write offset */

#define QSPI_WRITE_ENABLE_CMD        0x06u /**< Write Enable command */
#define QSPI_QUAD_ENABLE_BIT         0x2u  /**< Quad enable bit */

/***************************** Types Definitions *****************************/

typedef enum
{
    STATUS_REGISTER_1 = 0x00,
    STATUS_REGISTER_2 = 0x30,
    STATUS_REGISTER_3 = 0x10
} statusRegisterSelect_t;

/*************************** Variables Declarations **************************/

#if defined(CONFIG_MEMORY_QSPI_NOR)
extern uint32_t __qspi_start__;
extern uint32_t __qspi_end__;
#endif /* CONFIG_MEMORY_QSPI_NOR */

/*************************** Functions Declarations **************************/

extern returnCode_t QSPI_MemoryInit(void);
extern returnCode_t QSPI_MemoryRead(uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t QSPI_MemoryWrite(const uint8_t *data, uint32_t addr, uint32_t len);
extern returnCode_t QSPI_MemoryErase(uint32_t addr, uint32_t len);

#endif /* MEMDRV_QSPI_H */

/**
 * @}
 * @}
 * @}
 */
