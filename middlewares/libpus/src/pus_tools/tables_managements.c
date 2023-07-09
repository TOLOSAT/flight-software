/**
 * @file tables_management.h
 * @author Merlin Kooshmanian
 * @brief Source file for execution or routing tables
 * @date 09/07/2023
 *
 * Last Update : 09/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/**
 * @def  BUILD_ROUTING_KEY(apid, service, subservice)
 * @brief Preprocessor function that build routing key with APID, sevice and subservice
 */
#define BUILD_ROUTING_KEY(apid, service, subservice)    ((uint32_t)(apid << 16 | service << 8 | subservice))

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/
