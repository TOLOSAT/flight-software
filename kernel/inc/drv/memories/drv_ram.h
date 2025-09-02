/**
 * @file    drv_ram.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for RAM-emulated memory
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_ram RAM Memory Driver
 * @brief Abstraction layer for controlling an RAM-emulated memory.
 * @{
 */

#ifndef DRV_RAM_H
#define DRV_RAM_H

/******************************* Include Files *******************************/

#include "drv/common.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  ramConf_t
 * @brief   Struct type definition of a RAM-emulated memory configuration
 */
typedef struct
{
    void *base_addr; /**< @brief RAM-emulated memory base address (needs to be 512 bytes aligned) */
    uint32_t size;   /**< @brief RAM-emulated memory size (needs to be 512 bytes aligned) */
} ramConf_t;

/**
 * @struct  ramInst_t
 * @brief   Struct type definition of a RAM-emulated memory descriptor
 */
typedef struct
{
    const ramConf_t *p_conf; /**< @brief Pointer to RAM conf */
} ramInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t RamOpen(ramInst_t *ram_inst, const ramConf_t *const ram_conf);
extern returnCode_t RamWrite(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t RamRead(ramInst_t *ram_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t RamIoctl(ramInst_t *ram_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t RamClose(ramInst_t *ram_inst);

#endif /* DRV_RAM_H */

/**
 * @}
 * @}
 * @}
 */