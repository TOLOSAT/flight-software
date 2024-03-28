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

/***************************** Macros Defuploadions ****************************/

/***************************** Types Defuploadions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

void CheckSoftwareIntegrity(void);
void UploadSoftware(void);
void StartSoftware(void);

#endif /* BOOT_UPLOAD_H */
