/**
 * @file    tables_management.h
 * @author  Merlin Kooshmanian
 * @brief   Source file for execution or routing tables
 * @date    09/07/2023
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "kernel.h"
#include "tools/tables_management.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitRoutingTable(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size)
 * @brief       Check if the table is ordered from smallest to largest key
 * @param[in]   g_routing_table routing table we want to check
 * @param[in]   table_size Size of the routing table
 * @retval      #PUS_ERROR if the table is not ordered from smallest to largest key or cannot open device
 * @retval      #PUS_INVALID_PARAM if table size is 0 or if table is null pointer
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t InitRoutingTable(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((g_routing_table != NULL) && (table_size > 0u))
    {
        uint32_t i = 0u;
        uint32_t last_key = 0u;
        kernelStatus_t check_open = KERNEL_SUCCESSFUL;
        while((i < table_size) && (g_routing_table[i].key > last_key) && (check_open == KERNEL_SUCCESSFUL))
        {
            // Open the device for the route
            check_open = DeviceOpen(&g_routing_table[i].dev_route, DEVICE_TYPE_BUFFER, g_routing_table[i].route, DEVICE_NO_EXTRA_INFO);

            // Update last key
            last_key = g_routing_table[i].key;
            i++;
        }
        // Checks whether the entire table has been browsed
        // and if every device has been correctly opened.
        // If this is not the case, the table is not ordered.
        if((i != table_size) || (check_open != KERNEL_SUCCESSFUL))
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          InitExecutionTable(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size)
 * @brief       Check if the table is ordered from smallest to largest key
 * @param[in]   g_execution_table Execution table we want to check
 * @param[in]   table_size Size of the execution table
 * @retval      #PUS_ERROR if the table is not ordered from smallest to largest key
 * @retval      #PUS_INVALID_PARAM if table size is 0 or if table is null pointer
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t InitExecutionTable(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;

    // Function Core
    if ((g_execution_table != NULL) && (table_size > 0u))
    {
        uint32_t i = 0u;
        uint32_t last_key = 0u;
        while((i < table_size) && (g_execution_table[i].key > last_key))
        {
            last_key = g_execution_table[i].key;
            i++;
        }
        // Checks whether the entire table has been browsed. 
        // If this is not the case, the table is not ordered.
        if(i != table_size)
        {
            return_value = PUS_ERROR;
        }
    }
    else
    {
        return_value = PUS_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          RouteSearch(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size, uint32_t key, deviceNo_t *dev_route)
 * @brief       This function search for route in routing table with a key
 * @param[in]   g_routing_table Routing table where we search the route
 * @param[in]   table_size Size of the routing table
 * @param[in]   key Key that help us to find the route.
 * @param[out]  dev_route Device route we are looking for
 * @retval      #PUS_ERROR if key does not exist in routing table
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t RouteSearch(pusRoutingTable_t *g_routing_table, pusTableSize_t table_size, uint32_t key, deviceNo_t *dev_route)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_ERROR;
    pusTableSize_t left = 0u;
    pusTableSize_t right = table_size - 1u;
    pusTableSize_t cursor = left + (right - left) / 2u;

    // Function Core
    while ((left <= right) && (right < table_size) && (return_value != PUS_SUCCESSFUL))
    {
        if (g_routing_table[cursor].key == key)
        {
            *dev_route = g_routing_table[cursor].dev_route;
            return_value = PUS_SUCCESSFUL;
        }
        else if (g_routing_table[cursor].key < key)
        {
            left = cursor + 1u;
            cursor = left + (right - left) / 2u;
        }
        else
        {
            right = cursor - 1u;
            cursor = left + (right - left) / 2u;
        }
    }

    return return_value;
}

/**
 * @fn          ExecutionSearch(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size, uint32_t key, pusTMRequested_t *tm_requested, pusExecutionFunctionPtr_t *execution_function_ptr)
 * @brief       This function search for execution function in execution table with a key
 * @param[in]   g_execution_table Execution table where we search the function to execute
 * @param[in]   table_size Size of the execution table
 * @param[in]   key Key that help us to find the route.
 * @param[out]  execution_function_ptr Pointer to the function we want to execute
 * @param[out]  tm_requested Indicates if a specific TM has to be send for this TC
 * @retval      #PUS_ERROR if key does not exist in routing table
 * @retval      #PUS_SUCCESSFUL else
 */
pusStatus_t ExecutionSearch(pusExecutionTable_t *g_execution_table, pusTableSize_t table_size, uint32_t key, pusTMRequested_t *tm_requested, pusExecutionFunctionPtr_t *execution_function_ptr)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_ERROR;
    pusTableSize_t left = 0u;
    pusTableSize_t right = table_size - 1u;
    pusTableSize_t cursor = left + (right - left) / 2u;

    // Function Core
    while ((left <= right) && (right < table_size) && (return_value != PUS_SUCCESSFUL))
    {
        if (g_execution_table[cursor].key == key)
        {
            *execution_function_ptr = g_execution_table[cursor].execution_function;
            *tm_requested = g_execution_table[cursor].tm_requested;
            return_value = PUS_SUCCESSFUL;
        }
        else if (g_execution_table[cursor].key < key)
        {
            left = cursor + 1u;
            cursor = left + (right - left) / 2u;
        }
        else
        {
            right = cursor - 1u;
            cursor = left + (right - left) / 2u;
        }
    }

    return return_value;
}
