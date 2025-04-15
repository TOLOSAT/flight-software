/**
 * @file    sysinfo.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/sysinfo.h"
#include "system/context.h"
#include "system/console.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     STR
 * @brief   Convert a macro to a string
 */
#define STR(x)  #x // cppcheck-suppress misra-c2012-20.10; Exception: not used for critical code, just for pretty printing purpose

#define VERSION "v" STR(MAJOR) "." STR(MINOR) "." STR(PATCH) /**< Version string */

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_system_info
 * @brief   Constante struct containing system information
 */
const sytemInfo_t g_system_info = {
    .name       = SYSTEM_NAME,
    .version    = { .major = MAJOR, .minor = MINOR, .patch = PATCH },
    .build_type = BUILD_TYPE,
    .build_date = __DATE__,
    .build_time = __TIME__,
    .target     = BOARD,
};

/**
 * @var     g_program_name
 * @brief   Constant that holds program name
 */
const char *g_program_name = PROGRAM_NAME;

/*************************** Functions Definitions ***************************/

/**
 * @fn      PrintSystemInfo(void)
 * @brief   Function that prints all the system information onto the console
 * @return  Nothing
 */
void PrintSystemInfo(void)
{
    context_t context = { 0 };

    LOG("===============================================\n");
    LOG("Welcome on " PROGRAM_NAME "\n");
    LOG("System : " SYSTEM_NAME);
    LOG_DECIMAL("  Major : %d", MAJOR);
    LOG_DECIMAL("  Minor : %d", MINOR);
    LOG_DECIMAL("  Patch : %d", PATCH);
    LOG("  Build type " BUILD_TYPE "\n  Build on " __DATE__ " at " __TIME__ ", for " BOARD "\n");

    if (ReadContext(&context) == RET_SUCCESSFUL)
    {
        LOG("-----------------------------------------------\n");
        LOG("System context :\n");

        LOG_DECIMAL("  Boot count : %d\n", context.boot);
        LOG_DECIMAL("  Failed boot count : %d\n", context.failedBoot);

        LOG_HEXDECIMAL("  CFSR : %x\n", context.cfsr);
        LOG_HEXDECIMAL("  HFSR : %x\n", context.hfsr);
        LOG_HEXDECIMAL("  R0 : %x\n", context.registers.r[0]);
        LOG_HEXDECIMAL("  R1 : %x\n", context.registers.r[1]);
        LOG_HEXDECIMAL("  R2 : %x\n", context.registers.r[2]);
        LOG_HEXDECIMAL("  R3 : %x\n", context.registers.r[3]);
        LOG_HEXDECIMAL("  R12 : %x\n", context.registers.r12);
        LOG_HEXDECIMAL("  xPSR : %x\n", context.registers.xpsr);
        LOG_HEXDECIMAL("  LR : %x\n", context.registers.lr);
        LOG_HEXDECIMAL("  PC : %x\n", context.registers.pc);

        LOG("  Call stack :\n");
        for (uint32_t i = 0; i < context.callStack.last_idx; i++)
        {
            LOG_HEXDECIMAL("    %x\n", context.callStack.calls[i].lr);
        }
    }
    LOG("===============================================\n");
}