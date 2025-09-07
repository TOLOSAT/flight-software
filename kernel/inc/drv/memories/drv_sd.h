/**
 * @file    drv_sd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card memory using SDIO bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 * Adapted from STMicroelectronic example
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_sd SD Memory Driver
 * @brief Abstraction layer for controlling an SD card memory.
 * @{
 */

#ifndef DRV_SD_H
#define DRV_SD_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief SD handle struct type redefinition */
typedef SD_HandleTypeDef sdHandleStruct_t;

/** @brief SD peripheral type redefinition (SDMMC1, SDMMC2, ...) */
typedef SD_TypeDef sdPeriph_t;

/**
 * @struct  sdConf_t
 * @brief   Struct type definition of a SD configuration
 */
typedef struct
{
    sdPeriph_t *periph;        /**< @brief Pointer to the SD peripheral (SDMMC1, SDMMC2, ...) */
    IRQNo_t irq_no;            /**< @brief SD related interrupt */
    IRQPrio_t irq_prio;        /**< @brief SD related interrupt priority */
    uint32_t clock_edge;       /**< @brief SD clock signal transition */
    uint32_t clock_power_save; /**< @brief SD power save mode */
    uint32_t hw_flow_ctrl;     /**< @brief SD hardware flow control enable/disable */
    uint32_t bus_width;        /**< @brief SD bus width (1, 4, 8) */
    uint32_t clk_src;          /**< @brief SD peripheral clock source */
    uint32_t prescaler;        /**< @brief SD peripheral clock prescaler */
    IOConf_t io_cmd;           /**< @brief SD IO configuration for CMD */
    IOConf_t io_clk;           /**< @brief SD IO configuration for CLK */
    IOConf_t io_d0;            /**< @brief SD IO configuration for D0 */
    IOConf_t io_d1;            /**< @brief SD IO configuration for D1 */
    IOConf_t io_d2;            /**< @brief SD IO configuration for D2 */
    IOConf_t io_d3;            /**< @brief SD IO configuration for D3 */
} sdConf_t;

/**
 * @struct  sdInst_t
 * @brief   Struct type definition of a SD descriptor
 */
typedef struct
{
    sdHandleStruct_t handle_struct; /**< @brief SD handle struct used by HAL */
    const sdConf_t *p_conf;         /**< @brief Pointer to SD conf */
} sdInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t SdOpen(sdInst_t *sd_inst, const sdConf_t *const sd_conf);
extern returnCode_t SdWrite(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t SdRead(sdInst_t *sd_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t SdIoctl(sdInst_t *sd_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t SdClose(sdInst_t *sd_inst);

#endif /* DRV_SD_H */

/**
 * @}
 * @}
 * @}
 */