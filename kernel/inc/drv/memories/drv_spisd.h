/**
 * @file    drv_spisd.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card memory over SPI bus
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_spisd SD Memory Driver over SPI
 * @brief Abstraction layer for controlling an SD card memory over SPI bus.
 * @{
 */

#ifndef DRV_SPISD_H
#define DRV_SPISD_H

/******************************* Include Files *******************************/

#include "drv/common.h"
#include "drv/peripherals/drv_spi.h"
#include "drv/peripherals/drv_gpio.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    spisdCardType_t
 * @brief   SD card type type enum
 */
typedef enum
{
    NOT_SDCARD  = 0u, /**< @brief Not an SD Card */
    SDCARD_V1   = 1u, /**< @brief Is a SD card v1 */
    SDCARD_V2   = 2u, /**< @brief Is a SD card v2 */
    SDCARD_V2HC = 3u, /**< @brief Is a SD card v2 High Capacity */
} spisdCardType_t;

/**
 * @struct  spisdConf_t
 * @brief   Struct type definition of a SPISD configuration
 */
typedef struct
{
    spiPeriph_t *spi_periph; /**< @brief Pointer to the SPI peripheral (SPI1, SPI2, ...) */
    IRQNo_t irq_no;          /**< @brief SPISD related interrupt */
    IRQPrio_t irq_prio;      /**< @brief SPISD related interrupt priority */
    uint32_t clk_src;        /**< @brief SPI peripheral clock source */
    uint32_t prescaler;      /**< @brief SPI peripheral clock prescaler */
    IOConf_t io_sck;         /**< @brief SPI IO configuration for SCK */
    IOConf_t io_miso;        /**< @brief SPI IO configuration MISO */
    IOConf_t io_mosi;        /**< @brief SPI IO configuration MOSI */
    IOConf_t io_cs;          /**< @brief SPI IO configuration CS */
} spisdConf_t;

/**
 * @struct  spisdInst_t
 * @brief   Struct type definition of a SPISD descriptor
 */
typedef struct
{
    spiHandleStruct_t spi_handle_struct; /**< @brief SPI handle struct used by HAL */
    gpioPort_t *cs_port;                 /**< @brief GPIO CS port */
    gpioPin_t cs_pin;                    /**< @brief GPIO CS pin */
    spisdCardType_t sd_type;             /**< @brief SD card type */
    const spisdConf_t *p_conf;           /**< @brief Pointer to SPISD conf */
} spisdInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t SpisdOpen(spisdInst_t *spisd_inst, const spisdConf_t *const spisd_conf);
extern returnCode_t SpisdWrite(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t SpisdRead(spisdInst_t *spisd_inst, memorySector_t sector, data_t data, length_t length);
extern returnCode_t SpisdIoctl(spisdInst_t *spisd_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t SpisdClose(spisdInst_t *spisd_inst);

#endif /* DRV_SPISD_H */

/**
 * @}
 * @}
 * @}
 */