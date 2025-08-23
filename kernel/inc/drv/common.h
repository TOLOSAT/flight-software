/**
 * @file    common.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for drivers common functions (e.g. HAL init)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup drv Drivers
 * @{
 */

#ifndef DRIVERS_H
#define DRIVERS_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/irq.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

#define DRV_MAX_DELAY               30u /**< Max delay (in milliseconds) for function that uses HAL timer */

/**
 * @def     DMA_FROM_STREAM
 * @brief   Macro to get the DMA peripheral from the stream
 */
#define DMA_FROM_STREAM(stream)     ((DMA_TypeDef *)(((uintptr_t)(stream)) & ~0x3FFUL))

/**
 * @def     IS_VALID_DRV_MODE
 * @brief   Check if mode is a valid driving mode
 */
#define IS_VALID_DRV_MODE(drv_mode) (((drv_mode) == POLLING_MODE) || ((drv_mode) == INTERRUPT_MODE) || ((drv_mode) == DMA_MODE))

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

/** @brief Clock source type definition */
typedef uint32_t clockSource_t;

/** @brief UART DMA handle struct type redefinition */
typedef DMA_HandleTypeDef DMAHandleStruct_t;

/** @brief UART DMA reference type redefinition (DMA1_Stream0, DMA1_Stream1, ...) */
typedef DMA_Stream_TypeDef DMARef_t;

/** @brief UART DMA channel type definition */
typedef uint32_t DMAChannel_t;

/** @brief UART DMA channel type definition */
typedef uint32_t DMADirection_t;

/** @brief DMA configuration struct type*/
typedef struct
{
    DMARef_t *ref;            /**< @brief DMA stream reference (DMA1_Stream0, ...) */
    DMAChannel_t channel;     /**< @brief DMA related channel */
    DMADirection_t direction; /**< @brief DMA direction (memory to periph, periph to memory or memory to memory) */
    IRQNo_t irq_no;           /**< @brief DMA interrupt */
} DMAConf_t;

/** @brief IO port redefinition type */
typedef GPIO_TypeDef *IOPort_t;

/** @brief IO pin redefinition type */
typedef uint32_t IOPin_t;

/** @brief IO mode redefinition type */
typedef uint32_t IOMode_t;

/** @brief IO pull redefinition type */
typedef uint32_t IOPull_t;

/** @brief IO speed redefinition type */
typedef uint32_t IOSpeed_t;

/** @brief IO pin alternate function redefinition type*/
typedef uint32_t IOAlternate_t;

/** @brief IO configuration struct type */
typedef struct
{
    IOPort_t port;           /**< @brief IO port */
    IOPin_t pin;             /**< @brief IO pin */
    IOMode_t mode;           /**< @brief IO mode (push-pull, open-drain, input, output, ...) */
    IOPull_t pull;           /**< @brief IO pull (pull-up or pull-down resistor) */
    IOSpeed_t speed;         /**< @brief IO speed */
    IOAlternate_t alternate; /**< @brief IO alternate function */
} IOConf_t;

/** @brief Driver action callback type definition */
typedef void (*DrvCallback_t)(void *param);

/** @brief Driver action callback parameter type definition */
typedef void *DrvCallbackParam_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void InitHal(void);
extern returnCode_t SetupIO(const IOConf_t *const io_conf);
extern returnCode_t SetUpDMA(DMAHandleStruct_t *dma_handle, const DMAConf_t *const dma_conf);

#endif /* DRIVERS_H */

/**
 * @}
 * @}
 */