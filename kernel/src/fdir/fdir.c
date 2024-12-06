/**
 * @file    fdir.c
 * @author  Théo Bessel & Merlin Kooshmanian
 * @brief   Interface for Failure Detection, Identification and Recovery (FDIR).
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "fdir/fdir.h"
#include "core/tasks.h"
#include "system/console.h"
#include "system/sysleds.h"
#include "utils/log.h"
#include "bsp.h"

/***************************** Macros Definitions ****************************/



/*************************** Functions Declarations **************************/

inline void __attribute__((always_inline)) SaveRegisters(debugInfo_t *debug_info);

inline void __attribute__((always_inline)) PrepareUnwind(call_t* last_call);

/*************************** Handlers Declarations ***************************/

extern void Reset_Handler(void);
extern void HardFault_Handler(void);
extern void MemManage_Handler(void);
extern void BusFault_Handler(void);
extern void UsageFault_Handler(void);

/*************************** Variables Definitions ***************************/

/**
 * @brief Contains all the debugging informations
 */
debugInfo_t debug_info = {0};

/**
 * @brief Contains the last call (fp + lr)
 */
call_t last_call = {0};

/*************************** Functions Definitions ***************************/

/**
 *  @fn     InitFDIR(void)
 *  @brief  Function that initialises the FDIR
 */
void InitFDIR(void)
{
    // Enables memory management, bus fault and usage fault exceptions
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk;
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
    __disable_irq();
    LEDErrorOn();

    while (1);
}

/**
 * @brief This function saves the registers of the processor when an error occured
 * @param[out] debug_info         The structure where to store the saved registers
 * @return Nothing
 */
inline void __attribute__((always_inline)) SaveRegisters(debugInfo_t* debug_info)
{
    __asm volatile (
        "tst lr, #4         \n" // Test bit 2 of EXC_RETURN; Z is set if lr[2] = 1
        "ite eq             \n" // If-Then-Else conditional execution
        "mrseq %[sp], msp   \n" // If equal (Z=1), move the value of MSP to r1
        "mrsne %[sp], psp   \n" // If not equal (Z=0), move the value of PSP to r1
        : [sp] "=r" (
            (*debug_info).registers
        )                       // Output operands
        :                       // No input operands
        :                       // Clobbered register
    );

    (*debug_info).cfsr = (uint32_t) SCB->CFSR;
    (*debug_info).hfsr = (uint32_t) SCB->HFSR;
}

/**
 * @brief This function save the unwind base context when executed in an error handler
 * @param[out] last_call        The context to save
 * @return Nothing
 */
inline void __attribute__((always_inline)) PrepareUnwind(call_t* last_call) {
    __asm volatile (
        "str r7, %[call_fp]        \n"
        "tst lr, #4                \n"
        "ite eq                    \n"  // If-Then-Else conditional execution
        "mrseq r0, msp             \n"  // If equal (Z=1), move the value of MSP to r1
        "mrsne r0, psp             \n"  // If not equal (Z=0), move the value of PSP to r1
        "ldr %[call_lr], [r0, #20] \n"  // Save lr (=*r0+20) into call_lr, #20 is the offset from the start of the frame
        : [call_fp] "=m" (
            last_call->fp
        ), [call_lr] "=r" (
            last_call->lr
        )
        // Output operands
        :                               // No input operands
        : "r0"                          // No clobbered register
    );
}

/*************************** Interruption Handlers ***************************/

/**
 * @brief This function handles Hard fault interrupt.
 */
void __attribute__((naked)) HardFault_Handler(void)
{
    while (1);
}

/**
 * @brief This function handles Memory management fault.
 */
void __attribute__((naked)) MemManage_Handler(void)
{
    while (1);
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault.
 */
void __attribute__((naked)) BusFault_Handler(void)
{
    while (1);
}

/**
 * @brief This function handles Undefined instruction or illegal state.
 */
void __attribute__((naked)) UsageFault_Handler(void)
{
    // Save the registers
    SaveRegisters(&debug_info);

    // Unwind the stack to etablish a stacktrace
    PrepareUnwind(&last_call);
    UnwindStack(&(debug_info.call_stack), last_call);

    while (1);
}
