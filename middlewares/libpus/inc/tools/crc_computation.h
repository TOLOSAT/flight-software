/**
 * @file    crc_computation.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for CRC computation
 * @date    01/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup crc_computation CRC Computation Functions
 * Functions that allows to compute CRC using lookup tables
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
 */