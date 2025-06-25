/**
 * @file    pus11.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 11 functions (Time-based Scheduling)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus11 PUS Service 11
 * @brief PUS service 11 (Time-based Scheduling) implementation
 * @{
 */

#ifndef PUS11_H
#define PUS11_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define PUS11_MAXIMUM_DATA           10u                                                     /**< PUS11 how many data can be stored */
#define PUS11_DATA_STATUS_SIZE       1u                                                      /**< PUS11 data status field size */
#define PUS11_MAXIMUM_DATA_SIZE      (PUS11_DATA_STATUS_SIZE + PUS11_ACTIVITY_DATA_MAX_SIZE) /**< PUS11 data field size */
#define PUS11_DATA_TABLE_INFO_SIZE   8u                                                      /**< PUS11 data table info size in bytes */
#define PUS11_DATA_TABLE_SIZE        (PUS11_DATA_TABLE_INFO_SIZE + (PUS11_MAXIMUM_DATA * PUS11_MAXIMUM_DATA_SIZE)) /**< PUS11 data table size in bytes */
#define PUS11_ACTIVITY_DATA_MAX_SIZE (TC_MAX_DATA_SIZE - CUC_TIME_SIZE) /**< Add Activity in time based schedule TC data size */

/***************************** Types Definitions *****************************/

/** @brief PUS11 data index type */
typedef uint32_t pus11DataIndex_t;

/**
 * @enum    pus11Status_t
 * @brief   PUS 11 status enum
 */
typedef enum
{
    PUS11_DISABLE = 0u, /**< PUS11 is disabled */
    PUS11_ENABLE  = 1u, /**< PUS11 is enabled */
} pus11Status_t;

/**
 * @enum    pus11DataStatus_t
 * @brief   Enum type for pus11 data status (available/unavailable)
 */
typedef enum
{
    PUS11_DATA_AVAILABLE   = 0u, /**< PUS11 data available */
    PUS11_DATA_UNAVAILABLE = 1u, /**< PUS11 data unavailable */
} pus11DataStatus_t;

/**
 * @struct  pus11Data_t
 * @brief   Struct type of a pus11 data
 */
typedef struct
{
    uint8_t status;                                 /**< @brief Indicates if data is available or not */
    uint8_t raw_data[PUS11_ACTIVITY_DATA_MAX_SIZE]; /**< @brief Raw data content */
} ATTR_BYTE_ALIGNED pus11Data_t;
ASSERT_SIZE(pus11Data_t, PUS11_MAXIMUM_DATA_SIZE)

/**
 * @struct  pus11DataTableInfo_t
 * @brief   Struct type of a pus11 data table information
 */
typedef struct
{
    uint32_t nb_data;             /**< @brief Indicates how many data are in data table */
    pus11DataIndex_t write_index; /**< @brief Write index of the data table */
} ATTR_BYTE_ALIGNED pus11DataTableInfo_t;
ASSERT_SIZE(pus11DataTableInfo_t, PUS11_DATA_TABLE_INFO_SIZE)

/**
 * @struct  pus11DataTable_t
 * @brief   Struct type of a pus11 data table
 */
typedef struct
{
    pus11DataTableInfo_t info;            /**< @brief Data table information */
    pus11Data_t data[PUS11_MAXIMUM_DATA]; /**< @brief List of data */
} ATTR_BYTE_ALIGNED pus11DataTable_t;
ASSERT_SIZE(pus11DataTable_t, PUS11_DATA_TABLE_SIZE)

/**
 * @struct  pus11Context_t
 * @brief   Struct type for pus11 context
 */
typedef struct
{
    pus11Status_t pus11_status;    /**< @brief PUS11 status */
    bufferNo_t buffer_delayed_tc;  /**< @brief Buffer where the delayed TC will be pushed */
    deviceNo_t dev_delayed_tc;     /**< @brief Device bound to the delayed TC buffer */
    fileNo_t fil_pus11_schedule;   /**< @brief File where the pus11 schedule is stored */
    fileNo_t fil_pus11_data;       /**< @brief File where the pus11 data table is stored */
    deviceNo_t dev_pus11_schedule; /**< @brief Device bound to the pus11 schedule */
    deviceNo_t dev_pus11_data;     /**< @brief Device bound to the pus11 data table */
} pus11Context_t;

/**
 * @struct  pusAddActivityTCDataField_t
 * @brief   Struct type for add activity data field
 */
typedef struct
{
    cucTime_t timestamp;                        /**< @brief Activity Timestamp */
    uint8_t data[PUS11_ACTIVITY_DATA_MAX_SIZE]; /**< @brief Activity Data (is a TC but currently dummy uint32) */
} ATTR_BYTE_ALIGNED pusAddActivityTCDataField_t;
ASSERT_SIZE(pusAddActivityTCDataField_t, TC_MAX_DATA_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t InitS11(pus11Context_t *pus11_context);
extern returnCode_t ReleaseDelayedTC(pus11Context_t *pus11_context, time_t *next_tc_release_date);
extern returnCode_t ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS11_H */

/**
 * @}
 * @}
 * @}
 */