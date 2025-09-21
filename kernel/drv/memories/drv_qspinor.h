/**
 * @file    drv_qspinor.h
 * @author  Théo Bessel
 * @author  Merlin Kooshmanian
 * @brief   Header file for QSPI_NOR flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_qspi_nor QSPI_NOR Memory Driver
 * @brief Abstraction layer for controlling an QSPI_NOR flash memory.
 * @{
 */

#ifndef DRV_QSPI_NOR_H
#define DRV_QSPI_NOR_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief QSPI_NOR handle struct type redefinition */
typedef QSPI_HandleTypeDef qspiHandleStruct_t;

/** @brief QSPI_NOR peripheral type redefinition (QSPI1, QSPI2, ...) */
typedef QUADSPI_TypeDef qspiPeriph_t;

/**
 * @struct  qspinorConf_t
 * @brief   Struct type definition of a QSPI_NOR configuration
 */
typedef struct
{
    qspiPeriph_t *periph; /**< @brief Pointer to the QSPI_NOR peripheral (QSPI1, QSPI2, ...) */
    IRQNo_t irq_no;       /**< @brief QSPI NOR related interrupt */
    IRQPrio_t irq_prio;   /**< @brief QSPI NOR related interrupt priority */
    uint32_t clk_src;     /**< @brief QSPI NOR peripheral clock source */
    uint32_t prescaler;   /**< @brief QSPI NOR peripheral clock prescaler */
    uint32_t page_size;   /**< @brief QSPI NOR page size in bytes */
    uint32_t sector_size; /**< @brief QSPI NOR sector size in bytes */
    uint32_t nb_sector;   /**< @brief QSPI NOR number of blocks */
    IOConf_t io_sck;      /**< @brief QSPI NOR IO configuration for SCK */
    IOConf_t io_d0;       /**< @brief QSPI NOR IO configuration for D0 */
    IOConf_t io_d1;       /**< @brief QSPI NOR IO configuration for D1 */
    IOConf_t io_d2;       /**< @brief QSPI NOR IO configuration for D2 */
    IOConf_t io_d3;       /**< @brief QSPI NOR IO configuration for D3 */
    IOConf_t io_cs;       /**< @brief QSPI NOR IO configuration for CS */
} qspinorConf_t;

/**
 * @struct  qspinorInst_t
 * @brief   Struct type definition of a QSPI_NOR descriptor
 */
typedef struct
{
    qspiHandleStruct_t handle_struct; /**< @brief QSPI_NOR handle struct used by HAL */
    const qspinorConf_t *p_conf;      /**< @brief Pointer to QSPI_NOR conf */
} qspinorInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t QspiNorOpen(qspinorInst_t *qspinor_inst, const qspinorConf_t *const qspi_nor_conf);
extern returnCode_t QspiNorWrite(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t QspiNorRead(qspinorInst_t *qspinor_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t QspiNorIoctl(qspinorInst_t *qspinor_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t QspiNorClose(qspinorInst_t *qspinor_inst);

#endif /* DRV_QSPI_NOR_H */

/**
 * @}
 * @}
 * @}
 */