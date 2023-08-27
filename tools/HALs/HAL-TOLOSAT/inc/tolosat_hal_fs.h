/**
 * @file    tolosat_hal_fs.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for TOLOSAT HAL File System functions
 * @date    18/08/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup hal_tolosat TOLOSAT HAL
 * @{
 * @defgroup drv_fs Driver FS
 * Functions that allows to drive File System
 * @{
 */

#ifndef TOLOSAT_HAL_FS_H
#define TOLOSAT_HAL_FS_H

/******************************* Include Files *******************************/

#include "tolosat_hal_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** 
 * @enum    SDCardStatus_t
 * @brief   SD card status type enum
 */
typedef enum
{
    SD_CARD_OFF = 0u,   /**< @brief SD is OFF */
    SD_CARD_ON = 1u,    /**< @brief SD is ON */
} SDCardStatus_t;

/** 
 * @struct  FsInst_t
 * @brief   Struct type definition of a FS instance
 */
typedef struct
{
    char disk_path[4];          /**< @brief FS disk path */
    Diskio_drvTypeDef  driver;  /**< @brief FS driver */
    FATFS file_system;          /**< @brief FS */
} FsInst_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

halStatus_t FsOpen(FsInst_t *fs_inst);

#endif /* TOLOSAT_HAL_FS_H */

/** 
 * @} 
 * @} 
 */