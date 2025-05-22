/**
 * @file    boot.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for bootloader
 *
 * @copyright Copyright (c) TOLOSAT
 */

/******************************* Include Files *******************************/

#include "boot.h"
#include "boot_init.h"
#include "boot_upload.h"
#include "boot_download.h"
#include "boot_misc.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn      main(void)
 * @brief   Bootloader main is the entrypoint of the code
 * @return  0
 */
int main(void)
{
    // Init Boot Software
    BootInit();

    // Check boot mode
    if (IsDownloadMode())
    {
        // Download mode specific initialisation
        DownloadModeInit();

        // Download mode procedure
        DownloadMode();
    }
    else
    {
        // Upload mode specific initialisation
        UploadModeInit();

        // Get the previous boot status
        GetBootStatus();

        // Check Software Integrity
        CheckSoftwareIntegrity();

        // Upload Software in memory
        UploadSoftware();

        // Update the boot status
        UpdateBootStatus();

        // De-init Boot Software
        BootDeInit();

        // Start newly updated software
        StartSoftware();
    }

    return 0;
}
