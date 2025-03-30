/**
 * @file    pus5.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 5 functions (Event Reporting)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus5 PUS Service 5
 * @brief PUS service 5 (Event Reporting) implementation
 * @{
 */

#ifndef PUS5_H
#define PUS5_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define EVENT_ID_SIZE     4u                                /**< Event ID size */
#define EVENT_DATA_SIZE   6u                                /**< Event data size */
#define EVENT_REPORT_SIZE (EVENT_ID_SIZE + EVENT_DATA_SIZE) /**< Event report size */

/***************************** Types Definitions *****************************/

/**
 * @enum    pusEventSeverity_t
 * @brief   PUS 5 event severity enum
 */
typedef enum
{
    PUS5_INFORMATIVE_EVENT     = 0u, /**< Informative event */
    PUS5_LOW_SEVERITY_EVENT    = 1u, /**< Low severity event */
    PUS5_MEDIUM_SEVERITY_EVENT = 2u, /**< Medium severity event */
    PUS5_HIGH_SEVERITY_EVENT   = 3u, /**< High severity event */
} pusEventSeverity_t;

/**
 * @struct  eventReport_t
 * @brief   Struct type for an event report
 */
typedef struct
{
    uint32_t EID;                  /**< @brief Event ID */
    uint8_t data[EVENT_DATA_SIZE]; /**< @brief Event data */
} ATTR_BYTE_ALIGNED eventReport_t;
ASSERT_SIZE(eventReport_t, EVENT_REPORT_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t BuildS5SS1234(pusTM_t *tm, pusEventSeverity_t severity, eventReport_t *report);

#endif /* PUS5_H */

/**
 * @}
 * @}
 * @}
 */