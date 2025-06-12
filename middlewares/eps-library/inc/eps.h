/**
 * @file    eps.h
 * @author  Matteo Planchet
 * @brief   Header file for EPS
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup eps-library EPS
 * @brief Communication with EPS via CSP
 * @{
 */

#ifndef EPS_H
#define EPS_H

/******************************* Include Files *******************************/

#include "kernel.h"
#include "structs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @struct epsInst_t
 * @brief Struct for EPS instance.
 */
typedef struct
{
    deviceNo_t dev_i2c;  /**< I2C device that will be used for the EPS (need to be initialise outside the driver) */
    uint8_t i2c_address; /**< I2C address of the EPS */
} epsInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

static returnCode_t GetHk1(epsInst_t *power_inst, hkparam_t *hk);

#endif /* EPS_H */

/**
 * @}
 * @}
 */