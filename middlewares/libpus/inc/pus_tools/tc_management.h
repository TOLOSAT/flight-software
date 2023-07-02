/**
 * @file tc_management.h
 * @author Merlin Kooshmanian
 * @brief Header file for TC management
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup tc_management Functions for TC management
 * Functions that allows to format tc the right way, verify 
 * their validity, delete them ...
 * @{
 */

#ifndef TC_MANAGEMENT_H
#define TC_MANAGEMENT_H

/***************************** Include Files *********************************/

#include "pus_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

pusStatus_t EraseTC(pusTC_t *tc);
pusStatus_t FormatTC(pusTC_t *tc);

#endif /* TC_MANAGEMENT_H */

/** @} */