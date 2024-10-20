/**
 * @file    sysinfo.c
 * @author  Merlin Kooshmanian
 * @brief   Source defining system information handling
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "system/sysinfo.h"
#include "system/console.h"
#include "utils/log.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_system_info
 * @brief   Constante struct containing system information
 */
const sytemInfo_t g_system_info =
{
    .name = SYSTEM_NAME,
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
    LOG("Welcome on "PROGRAM_NAME"\n");
    LOG("System : "SYSTEM_NAME"-"VERSION", type "BUILD_TYPE", build on "__DATE__" at "__TIME__", for "BOARD"\n");
}