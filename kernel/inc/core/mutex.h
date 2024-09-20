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
#include "utils/os.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Mutex Handle type */
typedef SemaphoreHandle_t mutexHandle_t;

/** @brief Mutex Reference number type */
typedef uint32_t mutexNo_t;

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

/*************************** Functions Declarations **************************/

extern kernelStatus_t CreateMutexes(void);
extern kernelStatus_t AcquireMutex(mutexNo_t mutex);
extern kernelStatus_t ReleaseMutex(mutexNo_t mutex);

#endif /* MUTEX_H */

/** 
 * @}
 * @}
 * @}
 */