/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for housekeeping
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup housekeeping Housekeeping
 * @brief Provide standard interface for task housekeeping
 * @{
 */

#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "core/time.h"
#include "core/buffers.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    hkStatus_t
 * @brief   PUS 3 HK status enum
 */
typedef enum
{
    HK_DISABLE = 0u,  /**< Housekeeping is disabled */
    HK_ENABLE = 1u,   /**< Housekeeping is enabled */
} hkStatus_t;

/**
 * @struct  hkDesc_t
 * @brief   Struct type of a task descriptor
 */
typedef struct
{
    hkId_t hkid;            /**< @brief HK ID */
    hkStatus_t hk_status;   /**< @brief HK Status (enable/disable) */
    uint32_t counter;       /**< @brief HK counter (increments when a HK is emitted, decrements when HK is collected) */
} hkDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_hk_desc_table
 * @brief   Configuration table where all housekeeping parameters are stored
 */
extern hkDesc_t g_hk_desc_table[NB_HK];

/*************************** Functions Declarations **************************/

extern returnCode_t EnableHK(hkId_t hkid);
extern returnCode_t DisableHK(hkId_t hkid);
extern returnCode_t EmitHK(hk_t *hk);
extern returnCode_t CollectHKs(void);

#endif /* HOUSEKEEPING_H */

/**
 * @}
 * @}
 * @}
 */