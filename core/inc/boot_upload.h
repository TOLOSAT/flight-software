/**
 * @file    boot_upload.h
 * @author  Merlin Kooshmanian
 * @brief   Header file uploadialising tools and HAL used by boot
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef BOOT_UPLOAD_H
#define BOOT_UPLOAD_H

/******************************* Include Files *******************************/

#include "boot_types.h"

/***************************** Macros Definitions ****************************/

/**
 * @def     PROGRAM_NAME_STR
 * @brief   Program name header string
 */
#define PROGRAM_NAME_STR                  "PROGRAM_NAME = "

/**
 * @def     PROGRAM_NAME_STR_SIZE
 * @brief   Program name header string size
 */
#define PROGRAM_NAME_STR_SIZE             (sizeof(PROGRAM_NAME_STR) - 1u)

/**
 * @def     VECTOR_TABLE_ADDR_STR
 * @brief   Vector table address header string
 */
#define VECTOR_TABLE_ADDR_STR             "VECT_TAB_ADDR = "

/**
 * @def     VECTOR_TABLE_ADDR_STR_SIZE
 * @brief   Vector table address header string size
 */
#define VECTOR_TABLE_ADDR_STR_SIZE        (sizeof(VECTOR_TABLE_ADDR_STR) - 1u)

/**
 * @def     BACKUP_PROGRAM_NAME_STR
 * @brief   Backup program name header string
 */
#define BACKUP_PROGRAM_NAME_STR           "BACKUP_PROGRAM_NAME = "

/**
 * @def     BACKUP_PROGRAM_NAME_STR_SIZE
 * @brief   Backup program name header string size
 */
#define BACKUP_PROGRAM_NAME_STR_SIZE      (sizeof(BACKUP_PROGRAM_NAME_STR) - 1u)

/**
 * @def     BACKUP_VECTOR_TABLE_ADDR_STR
 * @brief   Backup vector table address header string
 */
#define BACKUP_VECTOR_TABLE_ADDR_STR      "BACKUP_VECT_TAB_ADDR = "

/**
 * @def     BACKUP_VECTOR_TABLE_ADDR_STR_SIZE
 * @brief   Backup vector table address header string size
 */
#define BACKUP_VECTOR_TABLE_ADDR_STR_SIZE (sizeof(BACKUP_VECTOR_TABLE_ADDR_STR) - 1u)

/***************************** Types Definitions *****************************/

/**
 * @struct  bootConf_t
 * @brief   Struct type of a boot configuration
 */
typedef struct
{
    char program_file_path[FF_MAX_LFN];        /**< @brief Path to the file containing the program */
    uint32_t vect_tab_addr;                    /**< @brief Address of the vector table (used by interrupt and startup) */
    char backup_program_file_path[FF_MAX_LFN]; /**< @brief Backup path to the file containing the program */
    uint32_t backup_vect_tab_addr;             /**< @brief Backup address of the vector table (used by interrupt and startup) */
} bootConf_t;

/**
 * @struct  bootStatus_t
 * @brief   Struct type of a boot status
 */
typedef struct
{
    uint32_t boot_counter;                   /**< @brief Boot counter, increments at each boot */
    uint32_t last_vect_tab_addr;             /**< @brief Address of the vector table (used by interrupt and startup) */
    char last_program_file_path[FF_MAX_LFN]; /**< @brief Path to the file containing the program */
} bootStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void GetBootStatus(void);
extern void UpdateBootStatus(void);
extern void GetBootConf(void);
extern void CheckSoftwareIntegrity(void);
extern void UploadSoftware(void);
extern void StartSoftware(void);

#endif /* BOOT_UPLOAD_H */
