/**
 * @file endianness_mgmt.h
 * @author Merlin Kooshmanian
 * @brief Header file for CRC computation
 * @date 04/07/2023
 *
 * Last Update : 04/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef ENDIANNESS_MGMT_H
#define ENDIANNESS_MGMT_H

#include <stdint.h>

#define HALF_WORD_BYTE_SWAP(word)    (((0xff00 & word) >> 8u) | ((0x00ff & word) << 8u)) /**< Preprocessor function that swaps byte in uint16 variable */

#define ARRAY_TO_UINT16_BIG_ENDIAN(array) ((uint16_t)(array[0u] << 8 | array[1u])) /**< Preprocessor function that convert 2 bytes array into uint16 big-endian variable */

#endif /* ENDIANNESS_MGMT_H */
