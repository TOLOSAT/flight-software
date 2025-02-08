/**
 * @file    pus6.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 6 functions (Memory Management)
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup middlewares Middlewares
 * @{
 * @defgroup pus PUS Library
 * @{
 * @defgroup pus6 PUS Service 6
 * @brief PUS service 6 (Memory Management) implementation
 * @{
 */

#ifndef PUS6_H
#define PUS6_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define MEMORY_ID_SIZE           1u                                                                            /**< Memory ID size */
#define MEMORY_BASE_SIZE         1u                                                                            /**< Memory base size */
#define MEMORY_OFFSET_SIZE       4u                                                                            /**< Memory offset size */
#define MEMORY_LENGTH_SIZE       4u                                                                            /**< Memory data length size */
#define MEMORY_TC_DATA_DUMP_SIZE (MEMORY_ID_SIZE + MEMORY_BASE_SIZE + MEMORY_OFFSET_SIZE + MEMORY_LENGTH_SIZE) /**< Memory TC data dump size */
#define MEMORY_TC_DATA_LOAD_MAX_SIZE \
    (TC_MAX_DATA_SIZE - MEMORY_ID_SIZE - MEMORY_BASE_SIZE - MEMORY_OFFSET_SIZE - MEMORY_LENGTH_SIZE) /**< Memory maximum TC data load size */
#define MEMORY_TM_DATA_DUMP_MAX_SIZE \
    (TM_MAX_DATA_SIZE - MEMORY_ID_SIZE - MEMORY_BASE_SIZE - MEMORY_OFFSET_SIZE - MEMORY_LENGTH_SIZE) /**< Memory maximum TM data dump size */

/***************************** Types Definitions *****************************/

/**
 * @struct  pusTCLoadDataField_t
 * @brief   Struct type for memory load TC data field
 */
typedef struct
{
    uint8_t memory_id;                          /**< @brief Memory ID (= disk ID) that will be loaded in memory */
    uint8_t base;                               /**< @brief Data base (= file ref no) that will be loaded in memory */
    uint32_t offset;                            /**< @brief Data offset in base that will be loaded in memory */
    uint32_t length;                            /**< @brief Data length that will be loaded in memory */
    uint8_t data[MEMORY_TC_DATA_LOAD_MAX_SIZE]; /**< @brief Data that will be loaded in memory */
} ATTR_BYTE_ALIGNED pusTCLoadDataField_t;
ASSERT_SIZE(pusTCLoadDataField_t, TC_MAX_DATA_SIZE)

/**
 * @struct  pusTCDumpDataField_t
 * @brief   Struct type for memory dump TC data field
 */
typedef struct
{
    uint8_t memory_id; /**< @brief Memory ID (= disk ID) that will be loaded in memory */
    uint8_t base;      /**< @brief Data base (= file ref no) that will be loaded in memory */
    uint32_t offset;   /**< @brief Data offset in base that will be loaded in memory */
    uint32_t length;   /**< @brief Data length that will be loaded in memory */
} ATTR_BYTE_ALIGNED pusTCDumpDataField_t;
ASSERT_SIZE(pusTCDumpDataField_t, MEMORY_TC_DATA_DUMP_SIZE)

/**
 * @struct  pusTMDumpDataField_t
 * @brief   Struct type for memory dump TM data field
 */
typedef struct
{
    uint8_t memory_id;                          /**< @brief Memory ID (= disk ID) that will be dumped from memory */
    uint8_t base;                               /**< @brief Data base (= file ref no) that will be dumped from memory */
    uint32_t offset;                            /**< @brief Data offset in base that will be dumped from memory */
    uint32_t length;                            /**< @brief Data length that will be dumped from memory */
    uint8_t data[MEMORY_TM_DATA_DUMP_MAX_SIZE]; /**< @brief Data that will be dumped from memory */
} ATTR_BYTE_ALIGNED pusTMDumpDataField_t;
ASSERT_SIZE(pusTMDumpDataField_t, TM_MAX_DATA_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern returnCode_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);
extern returnCode_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code);

#endif /* PUS6_H */

/**
 * @}
 * @}
 * @}
 */
