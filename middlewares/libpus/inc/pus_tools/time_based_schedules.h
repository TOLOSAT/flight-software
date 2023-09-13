/**
 * @file    time_based_schedules.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for time based schedules
 * @date    12/09/2023
 *
 * @copyright Copyright (c) TOLOSAT 2023
 */

/**
 * @defgroup libpus PUS Library
 * @{
 * @defgroup time_based_schedules Time Based Schedules
 * Functions for time based schedules
 * @{
 */

#ifndef TIME_BASED_SCHEDULES_H
#define TIME_BASED_SCHEDULES_H

/******************************* Include Files *******************************/

#include "pus_types.h"

/***************************** Macros Definitions ****************************/

#define MAXIMUM_NODES_PER_SCHEDULE  10u         /**< Maximum number of nodes in a schedule */
#define ACTIVITY_SIZE               10u         /**< Activity size in bytes */
#define ACTIVITY_NODE_SIZE          20u         /**< Activity node size in bytes */
#define SCHEDULE_INFO_SIZE          16u         /**< Schedule info size in bytes */
#define UNEXISTING_NODE_INDEX       0xffffffffu /**< Used for unexisting node index */

/***************************** Types Definitions *****************************/

/**
 * @enum    pusActivityNodeStatus_t
 * @brief   Enum type for node status (available/unavailable)
 */
typedef enum
{
    ACTIVITY_NODE_UNAVAILABLE = 0u, /**< Node available */
    ACTIVITY_NODE_AVAILABLE = 1u,   /**< Node unavailable */
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
    uint16_t status;                /**< @brief Indicates if node is available or not */
    pusActivity_t activity;         /**< @brief Indicates if node is available or not */
    uint32_t next_node_index;       /**< @brief Next node index (according to their timestamp) */
    uint32_t previous_node_index;   /**< @brief Previous node index (according to their timestamp) */
} BYTE_ALIGNED pusActivityNode_t;
ASSERT_SIZE(pusActivityNode_t, ACTIVITY_NODE_SIZE)

/** 
 * @struct  pusScheduleInfo_t
 * @brief   Struct type for schedule information
 */
typedef struct {
    uint32_t write_index;           /**< @brief Write index of the schedule */
    uint32_t nb_activities;         /**< @brief Indicates how many activities are in schedule */
    uint32_t oldest_activity_index; /**< @brief Oldest activity index */
    uint32_t newest_activity_index; /**< @brief Newest activity index */
} pusScheduleInfo_t;
ASSERT_SIZE(pusScheduleInfo_t, SCHEDULE_INFO_SIZE)

/** 
 * @struct  pusSchedule_t
 * @brief   Struct type for time based schedule
 */
typedef struct {
    pusScheduleInfo_t info;                                         /**< @brief Schedule information */
    pusActivityNode_t activity_nodes[MAXIMUM_NODES_PER_SCHEDULE];   /**< @brief List of nodes */
} pusSchedule_t;

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

pusStatus_t PushActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity);
pusStatus_t PopActivityInSchedule(pusSchedule_t *schedule, pusActivity_t *activity);

#endif /* TIME_BASED_SCHEDULES_H */

/** 
 * @} 
 * @} 
 */