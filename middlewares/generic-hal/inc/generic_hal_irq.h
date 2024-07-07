/**
 * @file    generic_hal_irq.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for GENERIC HAL IRQ functions
 * @date    29/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup generic_hal GENERIC HAL
 * @{
 * @defgroup drv_irq Driver IRQ
 * Functions that allows to use IRQ independantly of which 
 * hardware or HAL is used
 * @{
 */

#ifndef GENERIC_HAL_IRQ_H
#define GENERIC_HAL_IRQ_H

/******************************* Include Files *******************************/

#include "generic_hal_types.h"

/***************************** Macros Definitions ****************************/

#define IN_IRQ_TEXT_SECTION     __attribute__((section(".text_irq")))   /**< IRQ functions goes in the .text_irq */
#define IN_IRQ_DATA_SECTION     __attribute__((section(".data_irq")))   /**< IRQ data goes in the .data_irq */

#define MAX_IRQS                255                                     /**< Maximum interrupts available on ARM */
#define IRQ_OFFSET              16                                      /**< This offset is used to switch from IPSR to IRQn */
#define MAX_GENERIC_IRQS        (MAX_IRQS - IRQ_OFFSET)                 /**< Maximum number of generically handled interrupts (excluding ARM exceptions) */

#define IRQ_NONE                (IRQNo_t)(-16)                          /**< IRQ numero if no IRQ */

/***************************** Types Definitions *****************************/

/** @brief IRQ Numero type redefinition */
typedef IRQn_Type IRQNo_t;

/** @brief IRQ Priority type redefinition */
typedef uint32_t IRQPrio_t;

/** @brief IRQ Handler type definition */
typedef void (*IRQHandler_t)(void *param);

/** @brief IRQ Handler Parameter type definition */
typedef void* IRQHandlerParam_t;

/** 
 * @enum    IRQState_t
 * @brief   Interrupt State type definition
 */
typedef enum
{
    IRQ_DISABLED = 0u,  /**< IRQ is disabled */
    IRQ_ENABLED = 1u,   /**< IRQ is enabled */
} IRQState_t;

/** 
 * @struct  IRQDesc_t
 * @brief   Interrupt Descriptor type definition
 */
typedef struct {
    IRQNo_t irq_no;                     /**< @brief Interrupt Numero */
    IRQPrio_t priority;                 /**< @brief Interrupt Priority */
    IRQState_t state;                   /**< @brief Interrupt State (enable/disable) */
    IRQHandler_t handler;               /**< @brief Interrupt Handler */
    IRQHandlerParam_t handler_param;    /**< @brief Interrupt Handler Parameter */
    uint32_t count;                     /**< @brief Interrupt Counter */
} IRQDesc_t;

/*************************** Variables Declarations **************************/

extern IRQDesc_t g_irq_table[MAX_GENERIC_IRQS];

/*************************** Functions Declarations **************************/

halStatus_t RequestIRQ(IRQNo_t irq_no, IRQPrio_t priority, IRQHandler_t handler, IRQHandlerParam_t handler_param);
halStatus_t EnableIRQ(IRQNo_t irq_no);
halStatus_t DisableIRQ(IRQNo_t irq_no);

#endif /* GENERIC_HAL_IRQ_H */

/** 
 * @} 
 * @} 
 */