/**
 * @file    drv_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining type for HAL
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 */

#ifndef DRV_TYPES_H
#define DRV_TYPES_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

#define DRV_MAX_DELAY 30u /**< Max delay (in milliseconds) for function that uses HAL timer */

/***************************** Types Definitions *****************************/

/**
 * @enum    drivingMode_t
 * @brief   Driving mode type enum
 */
typedef enum
{
    POLLING_MODE   = 0u, /**< Peripheral is driven in polling mode */
    INTERRUPT_MODE = 1u, /**< Peripheral is driven using interrupts */
    DMA_MODE       = 2u, /**< Peripheral is driven using DMA */
} drivingMode_t;

/** @brief UART DMA handle struct type redefinition */
typedef DMA_HandleTypeDef DMAHandleStruct_t;

/** @brief UART DMA reference type redefinition (DMA1_Stream0, DMA1_Stream1, ...) */
typedef DMA_Stream_TypeDef DMARef_t;

/** @brief UART DMA channel type definition */
typedef uint32_t DMAChannel_t;

/** @brief Driver action callback type definition */
typedef void (*DrvCallback_t)(void *param);

/** @brief Driver action callback parameter type definition */
typedef void *DrvCallbackParam_t;

#endif /* DRV_TYPES_H */

/**
 * @}
 * @}
 */