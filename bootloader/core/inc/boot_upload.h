/**
 * @file    boot_upload.h
 * @author  Merlin Kooshmanian
 * @brief   Header file uploadialising tools and HAL used by boot
 * @date    27/03/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BOOT_UPLOAD_H
#define BOOT_UPLOAD_H

/******************************* Include Files *******************************/

#include <stdint.h>
#include "ffconf.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     PROGRAM_NAME_STR
 * @brief   Program name header string 
 */
#define PROGRAM_NAME_STR                        "program name = "

/**
 * @def     PROGRAM_NAME_STR_SIZE
 * @brief   Program name header string size
 */
#define PROGRAM_NAME_STR_SIZE                   (sizeof(PROGRAM_NAME_STR) - 1u)

/**
 * @def     VECTOR_TABLE_ADDR_STR
 * @brief   Vector table address header string
 */
#define VECTOR_TABLE_ADDR_STR                   "vector table address = "

/**
 * @def     VECTOR_TABLE_ADDR_STR_SIZE
 * @brief   Vector table address header string size
 */
#define VECTOR_TABLE_ADDR_STR_SIZE              (sizeof(VECTOR_TABLE_ADDR_STR) - 1u)

/**
 * @def     BACKUP_PROGRAM_NAME_STR
 * @brief   Backup program name header string
 */
#define BACKUP_PROGRAM_NAME_STR                 "backup program name = "

/**
 * @def     BACKUP_PROGRAM_NAME_STR_SIZE
 * @brief   Backup program name header string size
 */
#define BACKUP_PROGRAM_NAME_STR_SIZE            (sizeof(BACKUP_PROGRAM_NAME_STR) - 1u)

/**
 * @def     BACKUP_VECTOR_TABLE_ADDR_STR
 * @brief   Backup vector table address header string
 */
#define BACKUP_VECTOR_TABLE_ADDR_STR            "backup vector table address = "

/**
 * @def     BACKUP_VECTOR_TABLE_ADDR_STR_SIZE
 * @brief   Backup vector table address header string size
 */
#define BACKUP_VECTOR_TABLE_ADDR_STR_SIZE       (sizeof(BACKUP_VECTOR_TABLE_ADDR_STR) - 1u)

/***************************** Types Definitions *****************************/

/** 
 * @struct  bootConf_t
 * @brief   Struct type of a boot configuration
 */
typedef struct
{
    char program_file_path[FF_MAX_LFN]; /**< @brief Path to the file containing the program */
    uint32_t vect_tab_addr; /**< @brief Address of the vector table (used by interrupt and startup) */
    char backup_program_file_path[FF_MAX_LFN]; /**< @brief Backup path to the file containing the program */
    uint32_t backup_vect_tab_addr; /**< @brief Backup address of the vector table (used by interrupt and startup) */
} bootConf_t;


/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void GetBootConf(void);
void CheckSoftwareIntegrity(void);
void UploadSoftware(void);
void StartSoftware(void);

#endif /* BOOT_UPLOAD_H */
