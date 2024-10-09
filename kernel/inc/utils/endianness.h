/**
 * @file    endianness.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for endianness
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup utils Utils
 * @{
 * @defgroup endianness Endianness
 * @brief Functions for handling byte order (endianness).
 * @{
 */

#ifndef ENDIANNESS_H
#define ENDIANNESS_H

#include "kernel_types.h"

/**
 * @def     HALF_WORD_BYTE_SWAP(half_word)
 * @brief   Preprocessor function that swaps byte in uint16 variable
 */
#define HALF_WORD_BYTE_SWAP(half_word) (((0xff00u & (half_word)) >> 8u) | \
                                        ((0x00ffu & (half_word)) << 8u))

/**
 * @def     WORD_BYTE_SWAP(word)
 * @brief   Preprocessor function that swaps byte in uint32 variable
 */
#define WORD_BYTE_SWAP(word) (((0xff000000u & (word)) >> 24u) | \
                              ((0x00ff0000u & (word)) >> 8u)  | \
                              ((0x0000ff00u & (word)) << 8u)  | \
                              ((0x000000ffu & (word)) << 24u))

/**
 * @def     DOUBLE_WORD_BYTE_SWAP(double_word)
 * @brief   Preprocessor function that swaps bytes in a uint64_t variable
 */
#define DOUBLE_WORD_BYTE_SWAP(double_word) ( \
    ((0xff00000000000000ull & (double_word)) >> 56) | \
    ((0x00ff000000000000ull & (double_word)) >> 40) | \
    ((0x0000ff0000000000ull & (double_word)) >> 24) | \
    ((0x000000ff00000000ull & (double_word)) >> 8)  | \
    ((0x00000000ff000000ull & (double_word)) << 8)  | \
    ((0x0000000000ff0000ull & (double_word)) << 24) | \
    ((0x000000000000ff00ull & (double_word)) << 40) | \
    ((0x00000000000000ffull & (double_word)) << 56))

/**
 * @def     BIG_ENDIAN_ARRAY_TO_UINT16(array)
 * @brief   Preprocessor function that convert big-endian 2 bytes array into uint16 variable
 */
#define BIG_ENDIAN_ARRAY_TO_UINT16(array, half_word) \
    do { \
        (half_word) = ((uint16_t)(array)[0] << 8)  | \
                 ((uint16_t)(array)[1]); \
    } while (0)

/**
 * @def     BIG_ENDIAN_ARRAY_TO_UINT32(array)
 * @brief   Preprocessor function that convert big-endian 4 bytes array into uint32 variable
 */
#define BIG_ENDIAN_ARRAY_TO_UINT32(array, word) \
    do { \
        (word) = ((uint32_t)(array)[0] << 24) | \
                 ((uint32_t)(array)[1] << 16) | \
                 ((uint32_t)(array)[2] << 8)  | \
                 ((uint32_t)(array)[3]); \
    } while (0)

/**
 * @def     BIG_ENDIAN_ARRAY_TO_UINT64(array, double_word)
 * @brief   Preprocessor function that converts a big-endian 8-byte array into a uint64_t variable
 */
#define BIG_ENDIAN_ARRAY_TO_UINT64(array, double_word) \
    do { \
        (double_word) = ((uint64_t)(array)[0] << 56) | \
                        ((uint64_t)(array)[1] << 48) | \
                        ((uint64_t)(array)[2] << 40) | \
                        ((uint64_t)(array)[3] << 32) | \
                        ((uint64_t)(array)[4] << 24) | \
                        ((uint64_t)(array)[5] << 16) | \
                        ((uint64_t)(array)[6] << 8)  | \
                        ((uint64_t)(array)[7]); \
    } while (0)

/**
 * @def     UINT16_TO_BIG_ENDIAN_ARRAY(half_word, array)
 * @brief   Preprocessor function that convert uint16 big-endian variable into a 2 bytes big endian array
 */
#define UINT16_TO_BIG_ENDIAN_ARRAY(half_word, array) \
    do { \
        (array)[0] = (uint8_t)(((half_word) >> 8) & 0xFF); \
        (array)[1] = (uint8_t)((half_word) & 0xFF); \
    } while (0)

/**
 * @def     UINT32_TO_BIG_ENDIAN_ARRAY(word, array)
 * @brief   Preprocessor function that convert uint32 big-endian variable into a 4 bytes big endian array
 */
#define UINT32_TO_BIG_ENDIAN_ARRAY(word, array) \
    do { \
        (array)[0] = (uint8_t)(((word) >> 24) & 0xFF); \
        (array)[1] = (uint8_t)(((word) >> 16) & 0xFF); \
        (array)[2] = (uint8_t)(((word) >> 8) & 0xFF); \
        (array)[3] = (uint8_t)((word) & 0xFF); \
    } while (0)

/**
 * @def     UINT64_TO_BIG_ENDIAN_ARRAY(double_word, array)
 * @brief   Preprocessor function that converts a uint64_t big-endian variable into an 8-byte big-endian array
 */
#define UINT64_TO_BIG_ENDIAN_ARRAY(double_word, array) \
    do { \
        (array)[0] = (uint8_t)(((double_word) >> 56) & 0xFF); \
        (array)[1] = (uint8_t)(((double_word) >> 48) & 0xFF); \
        (array)[2] = (uint8_t)(((double_word) >> 40) & 0xFF); \
        (array)[3] = (uint8_t)(((double_word) >> 32) & 0xFF); \
        (array)[4] = (uint8_t)(((double_word) >> 24) & 0xFF); \
        (array)[5] = (uint8_t)(((double_word) >> 16) & 0xFF); \
        (array)[6] = (uint8_t)(((double_word) >> 8) & 0xFF); \
        (array)[7] = (uint8_t)((double_word) & 0xFF); \
    } while (0)

#endif /* ENDIANNESS_H */

/** 
 * @}
 * @}
 * @}
 */