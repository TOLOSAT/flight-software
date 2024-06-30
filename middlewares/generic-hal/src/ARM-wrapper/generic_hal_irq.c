/**
 * @file    generic_hal_irq.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for GENERIC IRQ functions
 * @date    04/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_irq_table
 * @brief   Interrupt descriptor table
 */
IRQDesc_t g_irq_table[MAX_GENERIC_IRQS] = {0};

/*************************** Functions Definitions ***************************/

/**
 * @fn          RequestIRQ(IRQNb_t irq_number, IRQPrio_t priority, IRQHandler_t handler, void *handler_param)
 * @brief       This function setups a interrupt
 * @param[in]   irq_number      Interrupt number (as defined in CMSIS)
 * @param[in]   priority        Interrupt priority
 * @param[in]   handler         Interrupt handler
 * @param[in]   handler_param   Interrupt handler param
 * @retval      #GEN_HAL_INVALID_PARAM if irq number is not valid 
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
halStatus_t RequestIRQ(IRQNb_t irq_number, IRQPrio_t priority, IRQHandler_t handler, void *handler_param)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((irq_number > (IRQNb_t)0u) && (irq_number <= (IRQNb_t)MAX_GENERIC_IRQS))
    {
        // Initialise the irq descriptor
        g_irq_table[irq_number].irq_number = irq_number;
        g_irq_table[irq_number].handler = handler;
        g_irq_table[irq_number].handler_param = handler_param;
        g_irq_table[irq_number].count = 0;
        g_irq_table[irq_number].state = IRQ_ENABLED;
        g_irq_table[irq_number].priority = priority;

        // Set IRQ priority in NVIC
        NVIC_SetPriority(irq_number, priority);
        
        // Enable IRQ in NVIC
        NVIC_EnableIRQ(irq_number);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          EnableIRQ(IRQNb_t irq_number)
 * @brief       Enable the interrupt
 * @param[in]   irq_number 
 * @retval      #GEN_HAL_INVALID_PARAM if irq number is not valid 
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
halStatus_t EnableIRQ(IRQNb_t irq_number)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((irq_number > (IRQNb_t)0u) && (irq_number <= (IRQNb_t)MAX_GENERIC_IRQS))
    {
        g_irq_table[irq_number].state = IRQ_ENABLED;
        NVIC_EnableIRQ(irq_number);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DisableIRQ(IRQNb_t irq_number)
 * @brief       Disable the interrupt
 * @param[in]   irq_number 
 * @retval      #GEN_HAL_INVALID_PARAM if irq number is not valid 
 * @retval      #GEN_HAL_SUCCESSFUL else
 */
halStatus_t DisableIRQ(IRQNb_t irq_number)
{
    // Variable Initialisation
    halStatus_t return_value = GEN_HAL_SUCCESSFUL;

    // Function Core
    if ((irq_number > (IRQNb_t)0u) && (irq_number <= (IRQNb_t)MAX_GENERIC_IRQS))
    {
        g_irq_table[irq_number].state = IRQ_DISABLED;
        NVIC_EnableIRQ(irq_number);
    }
    else
    {
        return_value = GEN_HAL_INVALID_PARAM;
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief Generic IRQ Handler
 */
void Generic_IRQHandler(void)
{
    // First get the IPSR that indicates which interrupts has been triggered
    int32_t ipsr = __get_IPSR();

    // Check if there is an interrupt (IPSR != 0) the current 
    // interrupt is not an ARM exception (IPSR = 1 ... 15)
    if ((ipsr >= IRQ_OFFSET) && (ipsr <= MAX_IRQS)) 
    {
        // Get IRQ number and 
        IRQNb_t irq_number = (IRQNb_t)(ipsr - 16);
        IRQDesc_t *irq_desc = &g_irq_table[irq_number];

        // Check if the interrupt is enabled before doing anything
        if (irq_desc->state == IRQ_ENABLED) 
        {
            // Increments counter
            irq_desc->count++;

            // If an handler is available call handler
            if (irq_desc->handler != NULL) 
            {
                irq_desc->handler(irq_desc->handler_param);
            }
        }
    }
    else
    {
        // Error : shouldn't be here
        while (1)
        {
            // Wait until the watchdog kills us
        }
    }
}
