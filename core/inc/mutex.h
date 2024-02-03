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

#include <stdint.h>

#include "os.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    mutexStatus_t
 * @brief   Mutex functions specific returns 
 */
typedef enum
{
    MUTEX_SUCCESSFUL            = 0u,    /**< Function succeed */
    MUTEX_ERROR                 = 1u,    /**< Function failed */
    MUTEX_INVALID_PARAM         = 2u,    /**< Function parameter is not valid */
    MUTEX_TIMEOUT               = 3u,    /**< Function returned a timeout */
} mutexStatus_t;

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

mutexStatus_t CreateMutexes(void);
mutexStatus_t AcquireMutex(mutexRef_t mutex);
mutexStatus_t ReleaseMutex(mutexRef_t mutex);

#endif /* MUTEX_H */

/** 
 * @} 
 * @} 
 */