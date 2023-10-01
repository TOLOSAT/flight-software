/**
 * @file    sd_crc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for SD card protocol CRCs
 * @date    01/10/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef SD_CRC_H
#define SD_CRC_H

/******************************* Include Files *******************************/

#include "tolosat_fs_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

extern const uint8_t g_sd_crc7_lookup_table[256];

/*************************** Functions Declarations **************************/

uint8_t ComputeCommandCRC7(const uint8_t *cmd_msg);

#endif /* SD_CRC_H */
