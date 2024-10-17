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
 * @defgroup utils Utils
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

#define HK_MESSAGE_SIZE   14u     /**< Size of a housekeeping message */

/***************************** Types Definitions *****************************/

/** @brief HK reference number type */
typedef uint32_t hkRef_t;

/** @brief HK reference number type */
typedef uint32_t hkId_t;

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
 * @struct  hkConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{                    
    hkRef_t ref;            /**< @brief HK reference number as it is declared in HK_ENUM */
    hkId_t hkid;            /**< @brief HK ID */
    hkStatus_t hk_status;   /**< @brief HK Status (enable/disable) */
} hkConf_t;

/** 
 * @struct  hkMessage_t
 * @brief   Struct type of an housekeeping message
 */
typedef struct
{                            
    uint8_t task_ref;       /**< @brief Task Reference Number */
    uint8_t observable;     /**< @brief Housekeeping Observable */
    uint32_t value;         /**< @brief Housekeeping Observable Value */
    time_t time;         /**< @brief Current Time */
}BYTE_ALIGNED hkMessage_t;
ASSERT_SIZE(hkMessage_t, HK_MESSAGE_SIZE)

/*************************** Variables Declarations **************************/

/**
 * @var     g_hk_desc_table
 * @brief   Configuration table where all housekeeping parameters are stored
 */
extern hkConf_t g_hk_desc_table[NB_HK];

/*************************** Functions Declarations **************************/

extern returnCode_t SendHkMessage(hkMessage_t *hk_message, bufferNo_t buffer);
extern returnCode_t ReceiveHkMessage(hkMessage_t *hk_message, bufferNo_t buffer);

#endif /* HOUSEKEEPING_H */

/** 
 * @}
 * @}
 * @}
 */