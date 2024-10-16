/**
 * @file    sysinfo.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "utils/sysinfo.h"
#include "utils/log.h"
#include "utils/console.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_sys_info
 * @brief   Constante struct containing system information
 */
const sysInfo_t g_sys_info =
{
    .name = "TAPAS",
    .version = VERSION,
    .build_type = BUILD_TYPE,
    .build_date = __DATE__,
    .build_time = __TIME__,
    .target = BOARD,
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
    LOG("Welcome on ");
    LOG(g_program_name);
    LOG("\n");
    LOG("System : ");
    LOG(g_sys_info.name);
    LOG("-");
    LOG(g_sys_info.version);
    LOG(", type ");
    LOG(g_sys_info.build_type);
    LOG(", build on ");
    LOG(g_sys_info.build_date);
    LOG(" at ");
    LOG(g_sys_info.build_time);
    LOG(", for ");
    LOG(g_sys_info.target);
    LOG("\n");
}