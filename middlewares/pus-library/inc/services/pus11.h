/**
 * @file    pus11.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 11 functions (Time-based Scheduling)
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
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

#define PUS11_MAXIMUM_DATA              10u                                                                             /**< PUS11 how many data can be stored */
#define PUS11_DATA_STATUS_SIZE          1u                                                                              /**< PUS11 data status field size */
#define PUS11_MAXIMUM_DATA_SIZE         (PUS11_DATA_STATUS_SIZE + PUS11_ACTIVITY_DATA_MAX_SIZE)                         /**< PUS11 data field size */
#define PUS11_DATA_TABLE_INFO_SIZE      8u                                                                              /**< PUS11 data table info size in bytes */
#define PUS11_DATA_TABLE_SIZE           (PUS11_DATA_TABLE_INFO_SIZE + (PUS11_MAXIMUM_DATA * PUS11_MAXIMUM_DATA_SIZE))   /**< PUS11 data table size in bytes */

/***************************** Types Definitions *****************************/

/** @brief PUS11 data index type */
typedef uint32_t pus11DataIndex_t;

/**
 * @enum    pus11Status_t
 * @brief   PUS 11 status enum
 */
typedef enum
{
    PUS11_DISABLE = 0u,  /**< PUS11 is disabled */
    PUS11_ENABLE = 1u,   /**< PUS11 is enabled */
}  pus11Status_t;

/**
 * @enum    pus11DataStatus_t
 * @brief   Enum type for pus11 data status (available/unavailable)
 */
typedef enum
{
    PUS11_DATA_AVAILABLE = 0u,   /**< PUS11 data available */
    PUS11_DATA_UNAVAILABLE = 1u, /**< PUS11 data unavailable */
} pus11DataStatus_t;

/** 
 * @struct  pus11Data_t
 * @brief   Struct type of a pus11 data
 */
typedef struct
{
    uint8_t status;                                 /**< @brief Indicates if data is available or not */
    uint8_t raw_data[PUS11_ACTIVITY_DATA_MAX_SIZE];  /**< @brief Raw data content */
} BYTE_ALIGNED pus11Data_t;
ASSERT_SIZE(pus11Data_t, PUS11_MAXIMUM_DATA_SIZE)

/** 
 * @struct  pus11DataTableInfo_t
 * @brief   Struct type of a pus11 data table information
 */
typedef struct
{
    uint32_t nb_data;               /**< @brief Indicates how many data are in data table */
    pus11DataIndex_t write_index;   /**< @brief Write index of the data table */
} BYTE_ALIGNED pus11DataTableInfo_t;
ASSERT_SIZE(pus11DataTableInfo_t, PUS11_DATA_TABLE_INFO_SIZE)

/** 
 * @struct  pus11DataTable_t
 * @brief   Struct type of a pus11 data table
 */
typedef struct
{
    pus11DataTableInfo_t info;              /**< @brief Data table information */
    pus11Data_t data[PUS11_MAXIMUM_DATA];   /**< @brief List of data */
} BYTE_ALIGNED pus11DataTable_t;
ASSERT_SIZE(pus11DataTable_t, PUS11_DATA_TABLE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t InitPus11(void);
extern pusStatus_t ExecuteS11SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t ExecuteS11SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t ExecuteS11SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t ExecuteS11SS4(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern pusStatus_t GetDelayedTC(pusTC_t *delayed_tc);

#endif /* PUS11_H */

/** 
 * @}
 * @}
 * @}
 */