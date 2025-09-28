/**
 * @file    sysinfo.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup sysinfo System Information
 * @brief Provides hardcoded system information.
 * @{
 */

#ifndef SYSINFO_H
#define SYSINFO_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct  sytemInfo_t
 * @brief   Struct that contains information about the system
 */
typedef struct
{
    const char *name;                /**< @brief System Name */
    const softwareVersion_t version; /**< @brief System Version */
    const char *build_type;          /**< @brief Build Type (debug/release) */
    const char *build_date;          /**< @brief Build Date */
    const char *build_time;          /**< @brief Build Time */
    const char *target;              /**< @brief Target */
} sytemInfo_t;

/*************************** Variables Declarations **************************/

extern const sytemInfo_t g_system_info;
extern const char *g_program_name;

/*************************** Functions Declarations **************************/

extern void PrintSystemInfo(void);

#endif /* SYSINFO_H */

/**
 * @}
 * @}
 * @}
 */