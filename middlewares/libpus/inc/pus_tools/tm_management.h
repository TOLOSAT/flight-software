/**
 * @file tm_management.h
 * @author Merlin Kooshmanian
 * @brief Header file for TM management
 * @date 02/07/2023
 *
 * Last Update : 02/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup tm_management Functions for TM management
 * Functions that allows to make tm the right way.
 * @{
 */

#ifndef TM_MANAGEMENT_H
#define TM_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

pusStatus_t BuildTM(pusTM_t *tm, pusService_t service, pusSubService_t subservice, pusData_t *data, uint16_t data_size);
pusStatus_t FormatTM(pusTM_t *tm);
pusStatus_t EraseTM(pusTM_t *tm);

#endif /* TM_MANAGEMENT_H */

/** @} */