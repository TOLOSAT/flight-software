/**
 * @file    boot_upload.h
 * @author  Théo Bessel
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

/***************************** Types Definitions *****************************/

/**
 * @struct  bootStatus_t
 * @brief   Struct type of a boot status
 */
typedef struct
{
    uint32_t boot_counter;    /**< Counter of reboot. */
} ATTR_PACKED bootStatus_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern void UploadModeInit(void);
extern void GetBootStatus(void);
extern void UpdateBootStatus(void);
extern void CheckSoftwareIntegrity(void);
extern void UploadSoftware(void);
extern void StartSoftware(void);

#endif /* BOOT_UPLOAD_H */
