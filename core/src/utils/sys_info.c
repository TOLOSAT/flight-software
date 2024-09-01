/**
 * @file    sys_info.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 * @date    01/06/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

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
void IN_CORE_TEXT_SECTION PrintSystemInfo(void)
{
    ConsolePrint("System : ");
    ConsolePrint(g_sys_info.name);
    ConsolePrint("-");
    ConsolePrint(g_sys_info.version);
    ConsolePrint(", type ");
    ConsolePrint(g_sys_info.build_type);
    ConsolePrint(", build on ");
    ConsolePrint(g_sys_info.build_date);
    ConsolePrint(" at ");
    ConsolePrint(g_sys_info.build_time);
    ConsolePrint(", for ");
    ConsolePrint(g_sys_info.target);
    ConsolePrint("\n");
}