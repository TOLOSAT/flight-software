/**
 * @file    bootloader_misc.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT bootloader types and defines
 * @date    02/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BOOTLOADER_MISC_H
#define BOOTLOADER_MISC_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include <stddef.h>
#include "ff_gen_drv.h"

#if defined(ART_PI)
#include "art_pi_bsp.h"
#elif defined(NUCLEO_H745ZI)
#include "nucleo_h745zi_bsp.h"
#else
#error "Board is not supported"
#endif

/***************************** Macros Definitions ****************************/

#define DISK0_REF                   0x00u       /**< Disk0 reference */

/***************************** Types Definitions *****************************/

/** @brief FS file numero type definition */
typedef uint32_t fsFileno_t;

/** @brief FS file Name type */
typedef const char fsfileName_t;

/** @brief FS file access mode type */
typedef uint8_t fsfileAccessMode_t;

/** @brief FS data type definition */
typedef uint8_t fsData_t;

/** @brief Size type definition */
typedef uint32_t fsSize_t;

/** @brief Timer instance type definition */
typedef TIM_HandleTypeDef timerInst_t;

/** 
 * @enum    halStatus_t
 * @brief   GENERIC HAL functions specific returns 
 */
typedef enum
{
    GEN_HAL_SUCCESSFUL = 0u,    /**< Function succeed */
    GEN_HAL_ERROR = 1u,         /**< Function failed */
    GEN_HAL_INVALID_PARAM = 2u, /**< Function parameter is not valid */
    GEN_HAL_TIMEOUT = 3u,       /**< Device timed out */
    GEN_HAL_BUSY = 4u,          /**< Device is busy */
} halStatus_t;

/** 
 * @enum    fsStatus_t
 * @brief   TOLOSAT file system functions specific returns 
 */
typedef enum
{
    FS_SUCCESSFUL = 0u,     /**< Function succeed */
    FS_ERROR = 1u,          /**< Function failed */
    FS_INVALID_PARAM = 2u,  /**< Function parameter is not valid */
    FS_UNAVAILABLE = 3u,    /**< Function is not available */
    FS_TIMEOUT = 4u,        /**< Device timed out */
    FS_BUSY = 5u,           /**< Device busy */
} fsStatus_t;

/** 
 * @struct  fsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];          /**< @brief FS disk path */
    Diskio_drvTypeDef  driver;  /**< @brief FS driver */
    FATFS file_system;          /**< @brief FS */
} fsInst_t;

/** 
 * @struct  fsFileConf_t
 * @brief   Struct type of a file configuration
 */
typedef struct
{
    fsFileno_t ref;                   /**< @brief File numero as it is declared in FILE_DEVICE_ENUM */                            
    fsfileName_t *name;               /**< @brief File name */
    fsfileAccessMode_t access_mode;   /**< @brief File access mode */
    FIL *temp_file;                   /**< @brief Pointer to the temporary file */
} fsFileConf_t;

#endif /* BOOTLOADER_MISC_H */
