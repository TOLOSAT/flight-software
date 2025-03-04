/**
 * @file    error_report.c
 * @author  Théo Bessel
 * @brief   Source file for Error Report API
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/error_report.h"
#include <ff.h>
#include <ffconf.h>
#include <ff_gen_drv.h>

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

returnCode_t ReadErrorReport(errorReport_t *report);
returnCode_t WriteErrorReport(errorReport_t report);

/*************************** Variables Definitions ***************************/

static FIL test_file_temp_file;

/*************************** Functions Definitions ***************************/

/**
 * @fn ReadErrorReport(errorReport_t *report)
 * @brief Read the error report of the kernel from the file system
 * @param[out] report Pointer to the error report structure
 * @retval      #RET_INVALID_PARAM if an error occurs while reading from the file system
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t ReadErrorReport(errorReport_t *report)
{
#if defined(CONFIG_FS_NONE)
    // Unused variables
    (void)report;

    // Always return successful
    return RET_SUCCESSFUL;
#else
    (void)report;
    return RET_SUCCESSFUL;
#endif /* CONFIG_FS_NONE */
}

/**
 * @fn WriteErrorReport(errorReport_t report)
 * @brief Save the error report of the kernel to the file system
 * @param[in] report Error report structure
 * @retval      #RET_INVALID_PARAM if an error occurs while writing to the file system
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t WriteErrorReport(errorReport_t report)
{
#if defined(CONFIG_FS_NONE)
    // Unused variables
    (void)report;

    // Always return successful
    return RET_SUCCESSFUL;
#else
    f_mkdir("/boot");
    f_open(&test_file_temp_file, "/boot/error_report.bin", FA_WRITE | FA_CREATE_ALWAYS);
    uint32_t bytes_written = 0u;
    f_write(&test_file_temp_file, &report, sizeof(report), (UINT *)&bytes_written);
    f_close(&test_file_temp_file);
    return RET_SUCCESSFUL;
#endif /* CONFIG_FS_NONE */
}