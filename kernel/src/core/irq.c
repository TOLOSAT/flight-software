/**
 * @file    irq.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for IRQ handling functions
 * @date    04/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/irq.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

#define IN_DESC_TABLES_SECTION  __attribute__((section(".desc_tables")))    /**< Descriptor table goes to .desc_tables section */

/*************************** Functions Declarations **************************/

extern void Generic_IRQHandler(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     g_irq_table
 * @brief   Interrupt descriptor table
 */
IRQDesc_t IN_DESC_TABLES_SECTION g_irq_table[MAX_GENERIC_IRQS] = 
{ 
    [0 ... (MAX_GENERIC_IRQS-1)] = {.irq_no = IRQ_NONE}
};

/*************************** Functions Definitions ***************************/

/**
 * @fn          RequestIRQ(IRQNo_t irq_no, IRQPrio_t priority, IRQHandler_t handler, IRQHandlerParam_t handler_param)
 * @brief       This function setups a interrupt
 * @param[in]   irq_no      Interrupt number (as defined in CMSIS)
 * @param[in]   priority        Interrupt priority
 * @param[in]   handler         Interrupt handler
 * @param[in]   handler_param   Interrupt handler param
 * @retval      #KERNEL_INVALID_PARAM if irq number is not valid 
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION RequestIRQ(IRQNo_t irq_no, IRQPrio_t priority, IRQHandler_t handler, IRQHandlerParam_t handler_param)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((int32_t) irq_no > 0)
    {
        // Initialise the irq descriptor
        g_irq_table[irq_no].irq_no = irq_no;
        g_irq_table[irq_no].handler = handler;
        g_irq_table[irq_no].handler_param = handler_param;
        g_irq_table[irq_no].count = 0;
        g_irq_table[irq_no].state = IRQ_ENABLED;
        g_irq_table[irq_no].priority = priority;

        // Set IRQ priority in NVIC
        NVIC_SetPriority(irq_no, priority);
        
        // Enable IRQ in NVIC
        NVIC_EnableIRQ(irq_no);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          EnableIRQ(IRQNo_t irq_no)
 * @brief       Enable the interrupt
 * @param[in]   irq_no 
 * @retval      #KERNEL_INVALID_PARAM if irq number is not valid 
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION EnableIRQ(IRQNo_t irq_no)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((int32_t) irq_no > 0)
    {
        g_irq_table[irq_no].state = IRQ_ENABLED;
        NVIC_EnableIRQ(irq_no);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          DisableIRQ(IRQNo_t irq_no)
 * @brief       Disable the interrupt
 * @param[in]   irq_no 
 * @retval      #KERNEL_INVALID_PARAM if irq number is not valid 
 * @retval      #KERNEL_SUCCESSFUL else
 */
kernelStatus_t IN_KERNEL_TEXT_SECTION DisableIRQ(IRQNo_t irq_no)
{
    // Variable Initialisation
    kernelStatus_t return_value = KERNEL_SUCCESSFUL;

    // Function Core
    if ((int32_t) irq_no > 0)
    {
        g_irq_table[irq_no].state = IRQ_DISABLED;
        NVIC_EnableIRQ(irq_no);
    }
    else
    {
        return_value = KERNEL_INVALID_PARAM;
    }

    return return_value;
}

/*************************** IRQ Handler Definition **************************/

/**
 * @brief Generic IRQ Handler
 */
void IN_KERNEL_TEXT_SECTION Generic_IRQHandler(void)
{
    // First get the IPSR that indicates which interrupts has been triggered
    IRQNo_t ipsr = (IRQNo_t)__get_IPSR();

    // Check if there is an interrupt (IPSR != 0) the current 
    // interrupt is not an ARM exception (IPSR = 1 ... 15)
    if (ipsr >= IRQ_OFFSET)
    {
        // Get IRQ number and 
        IRQNo_t irq_no = ipsr - 16u;
        IRQDesc_t *irq_desc = &g_irq_table[irq_no];

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
        __disable_irq();
        while (1)
        {
            // Wait until the watchdog kills us
        }
    }
}
