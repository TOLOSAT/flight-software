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
 * @defgroup core_functions Core Functions
 * @{
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
 * @enum    errorsSanction_t
 * @brief   Type of sanctions enum definition
 */
typedef enum
{
    FDIR_NO_SANCTION     = 0u,    /**< No sanction needed */
    FDIR_HALT_TASK       = 1u,    /**< Error needs current task to be halted (if possible) */
    FDIR_ERROR_HANDLER   = 2u,    /**< Error needs a reset of the system, but before system will save some data */
    FDIR_SYSTEM_RESET    = 3u,    /**< Error needs an immediate a reset of the system */
} errorsSanction_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void CheckErrors(uint32_t status, errorsSanction_t sanction);
void Error_Handler(void);

#endif /* ERRORS_MGMT_H */

/** 
 * @} 
 * @} 
 */