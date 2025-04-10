/**
 * @file    fdir.c
 * @author  Merlin Kooshmanian
 * @author  Théo Bessel
 * @brief   Source file for Failure Detection, Identification and Recovery (FDIR).
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "fdir/fdir.h"
#include "core/tasks.h"
#include "system/context.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static void SavePreExceptionRegisters(debugInfo_t *debug_info);
static void GetPreExceptionContext(call_t *context);
static void GetCurrentContext(call_t *context);
static void UpdateContext(void);

/*************************** Handlers Declarations ***************************/

extern void Reset_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/**
 * @var     debug_info
 * @brief   Contains all the debugging informations
 */
static debugInfo_t debug_info = { 0 };

/**
 * @var     context
 * @brief   Contains the context of the system
 */
static context_t context = { 0 };

/**
 * @var     last_call
 * @brief   Contains the last call (fp + lr)
 */
static call_t last_call = { 0 };

/*************************** Functions Definitions ***************************/

/**
 *  @fn     InitFDIR(void)
 *  @brief  Function that initialises the FDIR
 */
void InitFDIR(void)
{
    // Enables memory management, bus fault and usage fault exceptions
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk;
    // Enables division by 0 exception
    SCB->CCR |= SCB_CCR_DIV_0_TRP_Msk;
}

/**
 * @fn          CheckError(returnCode_t retcode)
 * @brief       This function check if an error occured and execute the sanction
 * @param[in]   retcode     Return code of a function.
 * @return      Nothing
 */
void CheckError(returnCode_t retcode)
{
    if (retcode == RET_ERROR)
    {
        // Indicates an error occured and system goes into error handler
        LOG("System : KO\n");

        // Go to error handler
        ErrorHandler();
    }
    else
    {
        // No errors
    }
}

/**
 * @fn      ErrorHandler(void)
 * @brief   This function is executed in case of error occurrence.
 * @warning Real FDIR has to be done.
 *
 * Normally this function has to save some context (what happened before error).
 * But currently no real fdir has been done. For debugging purposes, Error handler
 * is just a while loop that hangs processor indefinitly.
 */
void ErrorHandler(void)
{
    // Disable IRQ
    __disable_irq();

    // Warn that there is an error
    LEDErrorOn();

    // Infinite Loop
    while (1)
    {
        // Do Nothing
    }
}

/**
 * @fn KernelPanic(void)
 * @brief This function is executed in case of error occurrence in kernel space.
 * @warning WIP, not fully implemented now.
 *
 * This function saves the registers and proceeds a StackTrace
 */
void KernelPanic(void)
{
    // Disable IRQ
    __disable_irq();

    // Save the registers and context
    // TO DO : save registers
    GetCurrentContext(&last_call);

    // Warn that there is an error
    LEDErrorOn();

    // Unwind the stack to etablish a stacktrace
    UnwindStackFromContext(&(debug_info.call_stack), last_call);

    // Update the context
    UpdateContext();

    // Reboot the system
    NVIC_SystemReset();
}

/**
 * @fn          SavePreExceptionRegisters(debugInfo_t* debug_info)
 * @brief       This function saves the registers of the processor when an error occured
 * @param[out]  debug_info  The structure where to store the saved registers
 * @return      Nothing
 */
static ATTR_INLINE void SavePreExceptionRegisters(debugInfo_t *debug_info)
{
    __asm volatile("tst lr, #4         \n"              // Test bit 2 of EXC_RETURN; Z is set if lr[2] = 1
                   "ite eq             \n"              // If-Then-Else conditional execution
                   "mrseq %[sp], msp   \n"              // If equal (Z=1), move the value of MSP to r1
                   "mrsne %[sp], psp   \n"              // If not equal (Z=0), move the value of PSP to r1
                   : [sp] "=r"((*debug_info).registers) // Output operands
                   :                                    // No input operands
                   :                                    // Clobbered register
    );

    (*debug_info).cfsr = (uint32_t)SCB->CFSR;
    (*debug_info).hfsr = (uint32_t)SCB->HFSR;
}

/**
 * @fn          GetPreExceptionContext(call_t *context)
 * @brief       This function save the unwind base context when executed in an error handler
 * @param[out]  context   The context before exception occured.
 * @return      Nothing
 */
static ATTR_INLINE void GetPreExceptionContext(call_t *context)
{
    // Ignore unused parameters
    (void)(context);

    // Get pre-exception context
    __asm volatile("str r7, %[call_fp]        \n"
                   "tst lr, #4                \n"
                   "ite eq                    \n" // If-Then-Else conditional execution
                   "mrseq r0, msp             \n" // If equal (Z=1), move the value of MSP to r1
                   "mrsne r0, psp             \n" // If not equal (Z=0), move the value of PSP to r1
                   "ldr %[call_lr], [r0, #20] \n" // Save lr (=*r0+20) into call_lr, #20 is the offset from the start of the frame
                   : [call_fp] "=m"(context->fp),
                     [call_lr] "=r"(context->lr) // Output operands
                   :                             // No input operands
                   : "r0"                        // No clobbered register
    );
}

/**
 * @fn GetCurrentContext(call_t *context)
 * @brief This function save the unwind base context when executed in a function
 * @param[out]  context   The current context
 * @return      Nothing
 */
static ATTR_INLINE void GetCurrentContext(call_t *context)
{
    // Ignore unused parameters
    (void)(context);

    // Get pre-exception context
    __asm volatile("mov r0, pc                \n"
                   "str r7, %[call_fp]        \n"
                   "str r0, %[call_lr]        \n"
                   : [call_fp] "=m"(context->fp),
                     [call_lr] "=m"(context->lr) // Output operands
                   :                             // No input operands
                   : "r0"                        // No clobbered register
    );
}

/**
 * @fn UpdateContext(void)
 * @brief This function updates the context of the system
 * @return Nothing
 */
static ATTR_INLINE void UpdateContext(void)
{
    // Read the context
    ReadContext(&context);

    // Update the context
    context.state     = SOFTWARE_STATE_ERROR;
    context.cfsr      = debug_info.cfsr;
    context.hfsr      = debug_info.hfsr;
    context.registers = *(debug_info.registers);
    context.callStack = debug_info.call_stack;

    // Write the updated context
    WriteContext(&context);
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function handles Hard fault interrupt.
 */
void ATTR_EXCEPTION HardFault_Handler(void)
{
    // Save the registers
    SavePreExceptionRegisters(&debug_info);

    // Warn that there is an error
    LEDErrorOn();

    // Update the context
    UpdateContext();

    // Reboot the system
    NVIC_SystemReset();
}

/**
 * @brief This function handles Memory management fault.
 */
void ATTR_EXCEPTION MemManage_Handler(void)
{
    // Save the registers and context
    SavePreExceptionRegisters(&debug_info);
    GetPreExceptionContext(&last_call);

    // Warn that there is an error
    LEDErrorOn();

    // Unwind the stack to etablish a stacktrace
    UnwindStackFromContext(&(debug_info.call_stack), last_call);

    // Update the context
    UpdateContext();

    // Reboot the system
    NVIC_SystemReset();
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void ATTR_EXCEPTION BusFault_Handler(void)
{
    // Save the registers and context
    SavePreExceptionRegisters(&debug_info);
    GetPreExceptionContext(&last_call);

    // Warn that there is an error
    LEDErrorOn();

    // Unwind the stack to etablish a stacktrace
    UnwindStackFromContext(&(debug_info.call_stack), last_call);

    // Update the context
    UpdateContext();

    // Reboot the system
    NVIC_SystemReset();
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void ATTR_EXCEPTION UsageFault_Handler(void)
{
    // Save the registers and context
    SavePreExceptionRegisters(&debug_info);
    GetPreExceptionContext(&last_call);

    // Warn that there is an error
    LEDErrorOn();

    // Unwind the stack to etablish a stacktrace
    UnwindStackFromContext(&(debug_info.call_stack), last_call);

    // Update the context
    UpdateContext();

    // Reboot the system
    NVIC_SystemReset();
}
