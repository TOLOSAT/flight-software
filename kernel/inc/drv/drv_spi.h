/**
 * @file    drv_spi.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SPI functions
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 * @defgroup drv_spi SPI Driver
 * @brief Abstraction layer for controlling SPI buses.
 * @{
 */

#ifndef DRV_SPI_H
#define DRV_SPI_H

/******************************* Include Files *******************************/

#include "drv/drv_types.h"
#include "core/irq.h"

/***************************** Macros Definitions ****************************/

#define SPI_FILL_CHAR   0xffu   /**< SPI fill character */

/***************************** Types Definitions *****************************/

/** @brief SPI handle struct type redefinition */
typedef SPI_HandleTypeDef spiHandleStruct_t;

/** @brief SPI reference type redefinition (SPI1, SPI2, ...) */
typedef SPI_TypeDef spiRef_t;

/** @brief SPI prescaler (used to setup baudrate) type definition */
typedef uint32_t spiPrescaler_t;

/**
 * @enum    spiDriveType_t
 * @brief   SPI driving mode type enum
 */
typedef enum
{
    SPI_POLLING_MASTER_DRIVE = 0u, /**< SPI is driven in polling mode (CPU waits the data) and is bus master */
    SPI_POLLING_SLAVE_DRIVE = 1u,  /**< SPI is driven in polling mode (CPU waits the data) and is bus slave */
    SPI_IT_MASTER_DRIVE = 2u,      /**< SPI is driven by interrupts (CPU interrupts when there is data) and is bus master */
    SPI_IT_SLAVE_DRIVE = 3u,       /**< SPI is driven by interrupts (CPU interrupts when there is data) and is bus slave */
    SPI_DMA_MASTER_DRIVE = 4u,     /**< SPI is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus master (not available) */
    SPI_DMA_SLAVE_DRIVE = 5u,      /**< SPI is driven by DMA (when there is data DMA puts it in RAM without CPU call) and is bus slave (not available) */
} spiDriveType_t;

/**
 * @enum    spiReadType_t
 * @brief   SPI receive mode type enum
 */
typedef enum
{
    SPI_READ_RX_ONLY = 0u,  /**< SPI read does only a RX */
    SPI_READ_TX_RX = 1u,    /**< SPI read does TX and RX */
} spiReadType_t;

/**
 * @struct  spiInst_t
 * @brief   Struct type definition of a SPI instance
 */
typedef struct
{
    /** UART Handle, Reference and Interrupt */
    spiHandleStruct_t handle_struct;                /**< @brief SPI handle struct used by HAL */
    spiRef_t *spi_ref;                              /**< @brief SPI reference (SPI1, SPI2, ...) */
    IRQNo_t irq_no;                                 /**< @brief SPI related interrupt */
    /** Configuration Parameters */
    spiDriveType_t driving_mode;                    /**< @brief SPI drive mode as defining in spiDriveType_t enum */
    spiPrescaler_t prescaler;                       /**< @brief SPI precaler (used to setup baudrate)*/
    /** DMA */
    DMAHandleStruct_t dma_rx_handle_struct;         /**< @brief DMA RX handle struct used by HAL */
    DMAHandleStruct_t dma_tx_handle_struct;         /**< @brief DMA TX handle struct used by HAL */
    DMARef_t *dma_rx_ref;                           /**< @brief DMA RX reference (DMA1_Stream0, ...) */
    DMARef_t *dma_tx_ref;                           /**< @brief DMA TX reference (DMA1_Stream0, ...) */
    DMAChannel_t dma_rx_channel;                    /**< @brief DMA RX related channel */
    DMAChannel_t dma_tx_channel;                    /**< @brief DMA TX related channel */
    IRQNo_t dma_rx_irq_no;                          /**< @brief DMA RX interrupt */
    IRQNo_t dma_tx_irq_no;                          /**< @brief DMA TX interrupt */
    /** Callbacks */
    DrvCallback_t callback_rx_completed;            /**< @brief Callback when RX is completed */
    DrvCallbackParam_t callback_rx_completed_param; /**< @brief Callback parameter for RX completed */
    DrvCallback_t callback_tx_completed;            /**< @brief Callback when TX is completed */
    DrvCallbackParam_t callback_tx_completed_param; /**< @brief Callback parameter for TX completed */
} spiInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t SpiOpen(spiInst_t *spi_inst);
extern returnCode_t SpiWrite(spiInst_t *spi_inst, data_t msg, length_t length);
extern returnCode_t SpiRead(spiInst_t *spi_inst, data_t received_data, data_t transmit_data, length_t length);
extern returnCode_t SpiIoctl(spiInst_t *spi_inst, uint32_t cmd, void *data, uint32_t data_size);
extern returnCode_t SpiClose(spiInst_t *spi_inst);

#endif /* DRV_SPI_H */

/**
 * @}
 * @}
 * @}
 */