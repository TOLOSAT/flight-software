/**
 * @file    housekeeping.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for housekeeping
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup housekeeping Housekeeping
 * @brief Provide standard interface for housekeeping
 * @{
 */

#ifndef HOUSEKEEPING_H
#define HOUSEKEEPING_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/time.h"
#include "core/timers.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     HK_CONF(lineno)
 * @brief   Get hk conf from g_hk_conf_table
 */
#define HK_CONF(lineno)       (g_hk_conf_table[lineno])

/**
 * @def     HK_DESC(lineno)
 * @brief   Get hk conf from g_hk_desc_table
 */
#define HK_DESC(lineno)       (g_hk_desc_table[lineno])

/**
 * @def     IS_A_VALID_HK(lineno)
 * @brief   Indicates if the hk is valid
 */
#define IS_A_VALID_HK(lineno) ((HK_CONF(lineno).hkid != (hkId_t)NO_HK) && (HK_DESC(lineno).status == DESC_USED))

/***************************** Types Definitions *****************************/

/**
 * @enum    hkStatus_t
 * @brief   PUS 3 HK status enum
 */
typedef enum
{
    HK_DISABLE = 0u, /**< Housekeeping is disabled */
    HK_ENABLE  = 1u, /**< Housekeeping is enabled */
} hkStatus_t;

/**
 * @struct  hkConf_t
 * @brief   Struct type of a hk configuration
 */
typedef struct
{
    hkId_t hkid;                 /**< @brief HK ID */
    uint32_t *addr;              /**< @brief HK observable address */
    tick_t default_period;       /**< @biref HK default period */
    timerBuffer_t *p_tim_buffer; /**< @brief Pointer to the HK timer buffer */
} hkConf_t;

/**
 * @struct  hkDesc_t
 * @brief   Struct type of a hk descriptor
 */
typedef struct
{
    descStatus_t status;  /**< @brief Indicates if the descriptor is free or used */
    timerHandle_t handle; /**< @brief HK timer handle */
    hkStatus_t hk_status; /**< @brief HK Status (enable/disable) */
    hk_t last_hk;         /**< @brief Last HK value */
    bool valid;           /**< @brief Indicates if the last HK is valid or not */
} hkDesc_t;

/*************************** Variables Declarations **************************/

extern const hkConf_t g_hk_conf_table[CONFIG_MAX_NB_HKS];
extern hkDesc_t g_hk_desc_table[CONFIG_MAX_NB_HKS];

/*************************** Functions Declarations **************************/

extern returnCode_t EnableHK(hkId_t hkid);
extern returnCode_t DisableHK(hkId_t hkid);
extern returnCode_t GetLastHK(hkId_t hkid, hk_t *last_hk);

#endif /* HOUSEKEEPING_H */

/**
 * @}
 * @}
 * @}
 */