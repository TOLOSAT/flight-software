/**
 * @file fdir.h
 * @author Merlin Kooshmanian
 * @brief Error Management functions
 * @date 05/05/2023
 *
 * Last Update : 05/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup errors Errors Management
 * Functions that allows to manage errors in TAPAS
 * @{
 */

#ifndef ERRORS_MGMT_H
#define ERRORS_MGMT_H

/******************************* Include Files *******************************/

#include <stdint.h>

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @typedef errorsSanction_t
 * @brief   Type of sanctions enum definition
 */
typedef enum
{
    NO_SANCTION     = 0u,    /**< No sanction needed */
    ERROR_HANDLER   = 1u,    /**< Error has to be dealed in the error handler */
    RESET_HANDLER   = 2u,    /**< Error needs a reset of the system */
} errorsSanction_t;

/**************************** Functions Prototypes ***************************/

void CheckErrors(uint32_t status, errorsSanction_t sanction);
void Error_Handler(void);

#endif /* ERRORS_MGMT_H */

/** @} */