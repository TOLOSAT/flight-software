/**
 * @file    generic_hal_spi.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL SPI functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup generic_hal GENERIC HAL
 * @{
 * @defgroup drv_spi Driver SPI
 * Functions that allows to drive SPI
 * @{
 */

#ifndef GENERIC_HAL_SPI_H
#define GENERIC_HAL_SPI_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

#define SPI_FILL_CHAR   0xffu   /**< SPI fill character */

// Section placement macros
#define IN_SPI_TEXT_SECTION     __attribute__((section(".text_spi")))   /**< SPI functions goes in the .text_spi */
#define IN_SPI_DATA_SECTION     __attribute__((section(".data_spi")))   /**< SPI data goes in the .data_spi */

/***************************** Types Definitions *****************************/

/** @brief SPI handle struct type redefinition */
typedef SPI_HandleTypeDef spiHandleStruct_t;

/** @brief SPI reference type redefinition (SPI1, SPI2, ...) */
typedef SPI_TypeDef spiRef_t;

/** @brief SPI message type definition */
typedef uint8_t spiMsg_t;

/** @brief SPI message length type definition */
typedef uint16_t spiMsgLength_t;

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
    spiHandleStruct_t handle_struct;    /**< @brief SPI handle struct used by ST HAL */
    spiRef_t *spi_ref;                  /**< @brief SPI reference (SPI1, SPI2, ...) */
    spiDriveType_t drive_type;          /**< @brief SPI drive mode as defining in spiDriveType_t enum */
    spiPrescaler_t prescaler;           /**< @brief SPI precaler (used to setup baudrate)*/
} spiInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern halStatus_t SpiOpen(spiInst_t *spi_inst);
extern halStatus_t SpiWrite(spiInst_t *spi_inst, spiMsg_t *msg, spiMsgLength_t length);
extern halStatus_t SpiRead(spiInst_t *spi_inst, spiMsg_t *received_msg, spiMsg_t *transmit_msg, spiMsgLength_t length);
extern halStatus_t SpiIoctl(spiInst_t *spi_inst, halIoCtlCmd_t io_cmd);
extern halStatus_t SpiClose(spiInst_t *spi_inst);

#endif /* GENERIC_HAL_SPI_H */

/** 
 * @} 
 * @} 
 */