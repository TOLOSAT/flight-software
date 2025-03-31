/**
 * @file    sysinfo.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/sysinfo.h"
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
    LOG("Welcome on " PROGRAM_NAME "\n");
    LOG("System : " SYSTEM_NAME "-" VERSION ", type " BUILD_TYPE ", build on " __DATE__ " at " __TIME__ ", for " BOARD "\n");
}