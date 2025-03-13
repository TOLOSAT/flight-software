/**
 * @file    crc_computation.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CRC computation
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus_crc PUS CRC
 * @brief PUS Cyclic Redundancy Check (CRC) implementation.
 * @{
 */

#ifndef CRC_COMPUTATION_H
#define CRC_COMPUTATION_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern uint16_t computeCRC(const uint8_t *data, uint32_t length);

#endif /* CRC_COMPUTATION_H */

/**
 * @}
 * @}
 * @}
 */