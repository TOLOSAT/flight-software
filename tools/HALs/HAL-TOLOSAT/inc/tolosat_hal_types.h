/**
 * @file tolosat_hal_types.h
 * @author Merlin Kooshmanian
 * @brief Header defining type for HAL
 * @date 29/04/2023
 * 
 * Last Update : 29/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef TOLOSAT_HAL_TYPES_H
#define TOLOSAT_HAL_TYPES_H

/***************************** Include Files *********************************/

#include <stdint.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef enum {
    HAL_SUCCESSFUL      = 0u,    /**< Function succeed */
    HAL_ERROR           = 1u,    /**< Function failed*/
    HAL_INVALID_PARAM   = 2u,    /**< Function parameter is not valid**/
    HAL_TIMEOUT         = 3u,    /**< Function returned a timeout*/
} halStatus_t;


/************************** Function Prototypes ******************************/

#endif /* TOLOSAT_HAL_TYPES_H */