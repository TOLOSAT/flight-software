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

/**
 * @def HALF_WORD_BYTE_SWAP(half_word)
 * @brief Preprocessor function that swaps byte in uint16 variable
 */
#define HALF_WORD_BYTE_SWAP(half_word) (((0xff00 & half_word) >> 8u) | ((0x00ff & half_word) << 8u))

/**
 * @def WORD_BYTE_SWAP(word)
 * @brief Preprocessor function that swaps byte in uint32 variable
 */
#define WORD_BYTE_SWAP(word) (((0xff000000 & word) >> 24u) | ((0x00ff0000 & word) >> 8u) | ((0x0000ff00 & word) << 8u) | ((0x000000ff & word) << 24u))

/**
 * @def  ARRAY_TO_UINT16_BIG_ENDIAN(array)
 * @brief Preprocessor function that convert 2 bytes array into uint16 big-endian variable
 */
#define ARRAY_TO_UINT16_BIG_ENDIAN(array) ((uint16_t)(array[0u] << 8 | array[1u]))

#endif /* ENDIANNESS_MGMT_H */
