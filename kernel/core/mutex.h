/**
 * @file    mutex.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining mutexes
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup core Core
 * @{
 * @defgroup mutexes Mutexes
 * @brief Mutexes handling interface.
 * @{
 */

#ifndef MUTEX_H
#define MUTEX_H

/******************************* Include Files *******************************/

#include "autoconf.h"
#include "kernel_types.h"
#include "core/os.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     MUTEX_CONF(mutex_no)
 * @brief   Get mutex conf from g_mutexs_conf_table
 */
#define MUTEX_CONF(mutex_no) (g_mutexes_conf_table[(mutex_no) - 1u])

/**
 * @def     MUTEX_DESC(mutex_no)
 * @brief   Get mutex conf from g_mutexs_desc_table
 */
#define MUTEX_DESC(mutex_no) (g_mutexes_desc_table[(mutex_no) - 1u])

/**
 * @def     IS_A_VALID_MUTEX(mutex_no)
 * @brief   Indicates if the mutex_no is valid
 */
#define IS_A_VALID_MUTEX(mutex_no) \
    (((mutex_no) != (mutexNo_t)NO_MUTEX) && ((mutex_no) < (mutexNo_t)CONFIG_MAX_NB_MUTEXES) && (MUTEX_DESC(mutex_no).status == DESC_USED))

/***************************** Types Definitions *****************************/

/** @brief Mutex Handle type */
typedef SemaphoreHandle_t mutexHandle_t;

/**
 * @struct  mutexDesc_t
 * @brief   Struct type of a mutex descriptor
 */
typedef struct
{
    descStatus_t status;  /**< @brief Indicates if the descriptor is free or used */
    mutexHandle_t handle; /**< @brief Mutex handle */
} mutexDesc_t;

/*************************** Variables Declarations **************************/

extern const mutexConf_t g_mutexes_conf_table[CONFIG_MAX_NB_MUTEXES];
extern mutexDesc_t g_mutexes_desc_table[CONFIG_MAX_NB_MUTEXES];

/*************************** Functions Declarations **************************/

extern void CreateMutexes(void);
extern returnCode_t AcquireMutex(mutexNo_t mutex);
extern returnCode_t ReleaseMutex(mutexNo_t mutex);

#endif /* MUTEX_H */

/**
 * @}
 * @}
 * @}
 */