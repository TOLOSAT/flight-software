/**
 * @file    error_report.h
 * @author  Théo Bessel
 * @brief   Header file for Error Report API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup error_report Error Report
 * @brief Interface for saving and restoring kernel error reports.
 * @{
 */

#ifndef ERROR_REPORT_H
#define ERROR_REPORT_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "fdir/fdir.h"
#include "system/context.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct   errorReport_t
 * @brief    Error Report structure
 */
typedef struct
{
    context_t context;          /**< Context of the kernel at the time of the error. */
    savedRegisters_t registers; /**< Saved CPU registers.                            */
    uint32_t cfsr;              /**< Configurable Fault Status Register.             */
    uint32_t hfsr;              /**< Hard Fault Status Register.                     */
    callStack_t call_stack;     /**< Captured call stack.                            */
} ATTR_PACKED errorReport_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

returnCode_t ReadErrorReport(errorReport_t *report);
returnCode_t WriteErrorReport(errorReport_t report);

#endif /* ERROR_REPORT_H */

/**
 * @}
 * @}
 * @}
 */