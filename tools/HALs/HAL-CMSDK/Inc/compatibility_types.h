/**
 * @file    compatibility_types.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for HAL CMSDK types to be compatible with other HALs
 * @date    09/06/2024
 */

#ifndef COMPATIBILITY_TYPES_H
#define COMPATIBILITY_TYPES_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>

/***************************** Types Definitions *****************************/

/**
 * @brief I2C handle type for compatibilty (I2C is not supported by CMSDK)
 */
typedef uint32_t I2C_HandleTypeDef;

/**
 * @brief I2C type for compatibilty (I2C is not supported by CMSDK)
 */
typedef uint32_t I2C_TypeDef;

/**
 * @brief SPI handle type for compatibilty (SPI is not supported by CMSDK)
 */
typedef uint32_t SPI_HandleTypeDef;

/**
 * @brief SPI type for compatibilty (SPI is not supported by CMSDK)
 */
typedef uint32_t SPI_TypeDef;

#endif /* COMPATIBILITY_TYPES_H */