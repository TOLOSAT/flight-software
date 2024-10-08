/**
 * @file    mutex.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining mutexes
 * @date    28/10/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
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

#include "kernel_types.h"
#include "conf/mutex_conf.h"
#include "utils/os.h"

/***************************** Macros Definitions ****************************/

#define IN_MUTEX_QUEUE_SECTION  __attribute__((section(".mutex_queues")))   /**< Mutex queue go to .mutex_queues section */

/***************************** Types Definitions *****************************/

/** @brief Mutex Handle type */
typedef SemaphoreHandle_t mutexHandle_t;

/** @brief Mutex queue type */
typedef StaticSemaphore_t mutexQueue_t;

/** 
 * @struct  mutexConf_t
 * @brief   Struct type of a mutex configuration
 */
typedef struct
{                            
    mutexQueue_t *p_queue;      /**< @brief Pointer to the mutex queue */
} mutexConf_t;

/** 
 * @struct  mutexDesc_t
 * @brief   Struct type of a mutex descriptor
 */
typedef struct
{                            
    mutexHandle_t handle;   /**< @brief Mutex handle */
} mutexDesc_t;

/*************************** Variables Declarations **************************/

/**
 * @var     g_mutex_conf_table
 * @brief   Configuration table where all mutexes configuration are stored
 */
extern const mutexConf_t g_mutex_conf_table[NB_MUTEXES];

/**
 * @var     g_mutexes_desc_table
 * @brief   Configuration table where all mutexes descriptors are stored
 */
extern mutexDesc_t g_mutexes_desc_table[NB_MUTEXES];

/*************************** Functions Declarations **************************/

extern returnCode_t CreateMutexes(void);
extern returnCode_t AcquireMutex(mutexNo_t mutex);
extern returnCode_t ReleaseMutex(mutexNo_t mutex);

#endif /* MUTEX_H */

/** 
 * @}
 * @}
 * @}
 */