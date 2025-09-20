/**
 * @file    drv_qspimram.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for QSPI_MRAM flash memory using QSPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_qspi_mram QSPI_MRAM Memory Driver
 * @brief Abstraction layer for controlling an QSPI_MRAM flash memory.
 * @{
 */

#ifndef DRV_QSPI_MRAM_H
#define DRV_QSPI_MRAM_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief QSPI_MRAM handle struct type redefinition */
typedef QSPI_HandleTypeDef qspiHandleStruct_t;

/** @brief QSPI_MRAM peripheral type redefinition (QSPI1, QSPI2, ...) */
typedef QUADSPI_TypeDef qspiPeriph_t;

/**
 * @struct  qspimramConf_t
 * @brief   Struct type definition of a QSPI_MRAM configuration
 */
typedef struct
{
    qspiPeriph_t *periph; /**< @brief Pointer to the QSPI_MRAM peripheral (QSPI1, QSPI2, ...) */
    IRQNo_t irq_no;       /**< @brief QSPI MRAM related interrupt */
    IRQPrio_t irq_prio;   /**< @brief QSPI MRAM related interrupt priority */
    uint32_t clk_src;     /**< @brief QSPI MRAM peripheral clock source */
    uint32_t prescaler;   /**< @brief QSPI MRAM peripheral clock prescaler */
    uint32_t size;        /**< @brief QSPI MRAM size in bytes */
    bool qpi;             /**< @brief QSPI MRAM enable QPI mode (4-4-4) */
    IOConf_t io_sck;      /**< @brief QSPI MRAM IO configuration for SCK */
    IOConf_t io_d0;       /**< @brief QSPI MRAM IO configuration for D0 */
    IOConf_t io_d1;       /**< @brief QSPI MRAM IO configuration for D1 */
    IOConf_t io_d2;       /**< @brief QSPI MRAM IO configuration for D2 */
    IOConf_t io_d3;       /**< @brief QSPI MRAM IO configuration for D3 */
    IOConf_t io_cs;       /**< @brief QSPI MRAM IO configuration for CS */
} qspimramConf_t;

/**
 * @struct  qspimramInst_t
 * @brief   Struct type definition of a QSPI_MRAM descriptor
 */
typedef struct
{
    qspiHandleStruct_t handle_struct; /**< @brief QSPI_MRAM handle struct used by HAL */
    const qspimramConf_t *p_conf;     /**< @brief Pointer to QSPI_MRAM conf */
} qspimramInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t QspiMramOpen(qspimramInst_t *qspimram_inst, const qspimramConf_t *const qspi_mram_conf);
extern returnCode_t QspiMramWrite(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t QspiMramRead(qspimramInst_t *qspimram_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t QspiMramIoctl(qspimramInst_t *qspimram_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t QspiMramClose(qspimramInst_t *qspimram_inst);

#endif /* DRV_QSPI_MRAM_H */

/**
 * @}
 * @}
 * @}
 */