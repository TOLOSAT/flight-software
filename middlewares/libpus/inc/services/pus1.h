/**
 * @file pus1.c
 * @author Merlin Kooshmanian
 * @brief Header file for PUS 1 functions
 * @date 06/07/2023
 *
 * Last Update : 06/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup pus1 Generic functions used by PUS1
 * @{
 */

#ifndef PUS1_H
#define PUS1_H

/***************************** Include Files *********************************/

#include "pus_types.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

pusStatus_t SendS1SS1(pusTC_t *tc);
pusStatus_t SendS1SS2(pusTC_t *tc, pusAcceptanceError_t acceptance_error);

#endif /* PUS1_H */

/** @} */