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

#define ERROR_REPORT_FILE_PATH "/boot"            /**< Error report file path */
#define ERROR_REPORT_FILE_NAME "error_report.bin" /**< Error report file name */

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
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs           = FR_OK;

    test_fs = f_open(&test_file_temp_file, ERROR_REPORT_FILE_PATH "/" ERROR_REPORT_FILE_NAME, FA_READ);

    if (test_fs != FR_OK)
    {
        return_value = RET_ERROR;
    }
    else
    {
        uint32_t bytes_read = 0u;

        test_fs = f_read(&test_file_temp_file, report, sizeof(*report), (UINT *)&bytes_read);

        if (test_fs != FR_OK || bytes_read != sizeof(*report))
        {
            return_value = RET_ERROR;
        }
        else
        {
            test_fs = f_close(&test_file_temp_file);

            if (test_fs != FR_OK)
            {
                return_value = RET_ERROR;
            }
        }
    }

    return return_value;
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
    returnCode_t return_value = RET_SUCCESSFUL;
    FRESULT test_fs           = FR_OK;

    test_fs = f_mkdir(ERROR_REPORT_FILE_PATH);

    if (test_fs != FR_OK)
    {
        return_value = RET_ERROR;
    }
    else
    {
        test_fs = f_open(&test_file_temp_file, ERROR_REPORT_FILE_PATH "/" ERROR_REPORT_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE);

        if (test_fs != FR_OK)
        {
            return_value = RET_ERROR;
        }
        else
        {
            uint32_t bytes_written = 0u;

            test_fs = f_write(&test_file_temp_file, &report, sizeof(report), (UINT *)&bytes_written);

            if (test_fs != FR_OK || bytes_written != sizeof(report))
            {
                return_value = RET_ERROR;
            }
            else
            {
                test_fs = f_close(&test_file_temp_file);

                if (test_fs != FR_OK)
                {
                    return_value = RET_ERROR;
                }
            }
        }
    }

    return return_value;
#endif /* CONFIG_FS_NONE */
}