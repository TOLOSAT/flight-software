/**
 * @file    mutex.h
 * @author  Merlin Kooshmanian
 * @brief   Header file defining mutexes
 * @date    28/10/2023
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup core_functions Core Functions
 * @{
 * @defgroup mutex Mutex Management
 * Functions that allows to manage mutexes in TAPAS
 * @{
 */

#ifndef MUTEX_H
#define MUTEX_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "os.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief Mutex Handle type */
typedef SemaphoreHandle_t mutexHandle_t;

/** @brief Mutex Reference number type */
typedef uint32_t mutexRef_t;

/** 
 * @struct  mutexConf_t
 * @brief   Struct type of a mutex configuration
 */
typedef struct
{                            
    mutexRef_t ref;         /**< @brief Mutex reference number as it is declared in MUTEX_ENUM */
    mutexHandle_t handle;   /**< @brief Mutex handle */
} mutexConf_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

coreStatus_t CreateMutexes(void);
coreStatus_t AcquireMutex(mutexRef_t mutex);
coreStatus_t ReleaseMutex(mutexRef_t mutex);

#endif /* MUTEX_H */

/** 
 * @} 
 * @} 
 */