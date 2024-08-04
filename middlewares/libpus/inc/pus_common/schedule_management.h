/**
 * @file    schedule_management.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for schedules
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup schedule_management Schedules Management
 * Functions for schedules
 * @{
 */

#ifndef SCHEDULE_MANAGEMENT_H
#define SCHEDULE_MANAGEMENT_H

/******************************* Include Files *******************************/

#include "pus_types.h"
#include "fs.h"

/***************************** Macros Definitions ****************************/

#define MAXIMUM_ACTIVITIES_PER_SCHEDULE     10u                                                                             /**< Maximum number of nodes in a schedule */
#define ACTIVITY_SIZE                       10u                                                                             /**< Activity size in bytes */
#define ACTIVITY_NODE_SIZE                  20u                                                                             /**< Activity node size in bytes */
#define SCHEDULE_INFO_SIZE                  12u                                                                             /**< Schedule info size in bytes */
#define SCHEDULE_SIZE                       (SCHEDULE_INFO_SIZE + (MAXIMUM_ACTIVITIES_PER_SCHEDULE * ACTIVITY_NODE_SIZE))   /**< Schedule size in bytes */
#define UNEXISTING_NODE_INDEX               0xffffffffu                                                                     /**< Used for unexisting node index */

/***************************** Types Definitions *****************************/

/** @brief Node index type */
typedef uint32_t pusNodeIndex_t;

/**
 * @enum    pusActivityNodeStatus_t
 * @brief   Enum type for node status (available/unavailable)
 */
typedef enum
{
    ACTIVITY_NODE_AVAILABLE = 0u,   /**< Node available */
    ACTIVITY_NODE_UNAVAILABLE = 1u, /**< Node unavailable */
} pusActivityNodeStatus_t;

/** 
 * @struct  pusActivity_t
 * @brief   Struct type for time based activity
 */
typedef struct {
    cucTime_t timestamp;    /**< @brief Activity timestamp in second */
    uint32_t data;          /**< @brief Data linked to this activity (could be raw data or data index from data table) */
} BYTE_ALIGNED pusActivity_t;
ASSERT_SIZE(pusActivity_t, ACTIVITY_SIZE)

/** 
 * @struct  pusActivityNode_t
 * @brief   Struct type for activity based node
 */
typedef struct {
    uint16_t status;                    /**< @brief Indicates if node is available or not */
    pusActivity_t activity;             /**< @brief Indicates if node is available or not */
    pusNodeIndex_t next_node_index;     /**< @brief Next node index (according to their timestamp) */
    pusNodeIndex_t previous_node_index; /**< @brief Previous node index (according to their timestamp) */
} BYTE_ALIGNED pusActivityNode_t;
ASSERT_SIZE(pusActivityNode_t, ACTIVITY_NODE_SIZE)

/** 
 * @struct  pusScheduleInfo_t
 * @brief   Struct type for schedule information
 */
typedef struct {
    uint32_t nb_activities;                 /**< @brief Indicates how many activities are in schedule */
    pusNodeIndex_t write_index;             /**< @brief Write index of the schedule */
    pusNodeIndex_t oldest_activity_index;   /**< @brief Oldest activity index */
} pusScheduleInfo_t;
ASSERT_SIZE(pusScheduleInfo_t, SCHEDULE_INFO_SIZE)

/** 
 * @struct  pusSchedule_t
 * @brief   Struct type for time based schedule
 */
typedef struct {
    pusScheduleInfo_t info;                                             /**< @brief Schedule information */
    pusActivityNode_t activity_nodes[MAXIMUM_ACTIVITIES_PER_SCHEDULE];  /**< @brief List of nodes */
} pusSchedule_t;
ASSERT_SIZE(pusSchedule_t, SCHEDULE_SIZE)

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

extern pusStatus_t PushActivityInSchedule(fsFileno_t schedule_fileno, pusActivity_t *activity);
extern pusStatus_t PopActivityInSchedule(fsFileno_t schedule_fileno, pusActivity_t *activity);

#endif /* SCHEDULE_MANAGEMENT_H */

/** 
 * @} 
 * @} 
 */