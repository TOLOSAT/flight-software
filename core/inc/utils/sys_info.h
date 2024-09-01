/**
 * @file    sys_info.h
 * @author  Merlin Kooshmanian
 * @brief   Header defining system information handling
 * @date    01/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef SYS_INFO_H
#define SYS_INFO_H

/******************************* Include Files *******************************/

#include "core_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @struct  sysInfo_t
 * @brief   Struct that contains information about the system
 */
typedef struct
{
    const char *name;       /**< @brief System Name */
    const char *version;    /**< @brief System Version */
    const char *build_type; /**< @brief Build Type (debug/release) */
    const char *build_date; /**< @brief Build Date */
    const char *build_time; /**< @brief Build Time */
    const char *target;     /**< @brief Target */
} sysInfo_t;

/*************************** Variables Declarations **************************/

extern const sysInfo_t g_sys_info;
extern const char *g_program_name;

/*************************** Functions Declarations **************************/

extern void PrintSystemInfo(void);

#endif /* SYS_INFO_H */