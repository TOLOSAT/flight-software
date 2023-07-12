/**
 * @file tables_management.h
 * @author Merlin Kooshmanian
 * @brief Source file for execution or routing tables
 * @date 09/07/2023
 *
 * Last Update : 09/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/******************************* Include Files *******************************/

#include "pus_tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**************************** Functions Prototypes ***************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          RouteSearch(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size, uint32_t key, uint32_t *route)
 * @brief       This function search for route in routing table with a key
 * @param[in]   g_routing_table Routing table where we search the route
 * @param[in]   table_size Size of the routing table
 * @param[in]   key Key that help us to find the route.
 * @param[out]  route Route we are looking for
 * @retval      PUS_ERROR if key does not exist in routing table
 * @retval      PUS_SUCCESSFUL else
 */
pusStatus_t RouteSearch(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size, uint32_t key, uint32_t *route) 
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_ERROR;
    pusTableSize_t left = 0;
    pusTableSize_t right = table_size - 1;
    pusTableSize_t cursor = left + (right - left) / 2;;

    // Function Core
    while (left <= right && right < table_size && return_value != PUS_SUCCESSFUL) 
    {
        if (g_routing_table[cursor].key == key) 
        {
            *route = g_routing_table[cursor].route;
            return_value = PUS_SUCCESSFUL;
        }
        else if (g_routing_table[cursor].key < key)
        {
            left = cursor + 1;
            cursor = left + (right - left) / 2;
        }
        else
        {
            right = cursor - 1;
            cursor = left + (right - left) / 2;
        }
    }

    return return_value;
}

pusStatus_t ExecutionSearch(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size, uint32_t key, pusExecutionFunctionPtr_t *execution_function_ptr)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    *execution_function_ptr = g_execution_table[0].execution_function;

    return return_value;
}