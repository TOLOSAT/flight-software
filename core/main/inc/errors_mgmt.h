/**
 * @file errors_mgmt.h
 * @author Merlin Kooshmanian
 * @brief Error Management functions
 * @date 05/05/2023
 *
 * Last Update : 05/05/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef ERRORS_MGMT_H
#define ERRORS_MGMT_H

/***************************** Include Files *********************************/

#include <stdint.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum {
    NO_SANCTION     = 0u,    /**< No sanction needed */
    ERROR_HANDLER   = 1u,    /**< Error has to be dealed in the error handler */
    RESET_HANDLER   = 2u,    /**< Error needs a reset of the system */
} errorsSanction_t;

/************************** Function Prototypes ******************************/

#endif /* ERRORS_MGMT_H */