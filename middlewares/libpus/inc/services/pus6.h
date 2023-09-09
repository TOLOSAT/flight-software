/**
 * @file    pus6.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for PUS 6 functions (Memory management)
 * @date    08/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup pus6 PUS Service 6
 * Generic functions used by PUS6 (Memory management)
 * @{
 */

#ifndef PUS6_H
#define PUS6_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/** @brief File Name type */
typedef const char fileName_t;

/** @brief File Access mode type */
typedef uint8_t fileAccessMode_t;

/** 
 * @struct  pus6StaticConf_t
 * @brief   Struct type of a task configuration
 */
typedef struct
{                            
    fileName_t *name;               /**< @brief File name */
    fileAccessMode_t access_mode;   /**< @brief File access mode */
} pus6StaticConf_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t ExecuteS6SS1(pusTC_t *tc, pusTM_t *tm);
pusStatus_t ExecuteS6SS3(pusTC_t *tc, pusTM_t *tm);
pusStatus_t BuildS6SS4(pusTM_t *tm, pusTMDumpDataField_t *memory_dump);

#endif /* PUS6_H */

/** 
 * @} 
 * @} 
 */
