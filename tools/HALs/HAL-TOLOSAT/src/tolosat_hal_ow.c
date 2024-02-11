/**
 * @file    tolosat_hal_ow.c
 * @author  Merlin Kooshmanian
 * @brief   Source file for TOLOSAT HAL OW functions
 * @date    11/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "tolosat_hal.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_isnt)
 * @brief           Function that initialise an One Wire peripheral
 * @param[in,out]   ow_isnt Instance that contains One Wire parameters handlers
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwOpen(owInst_t *ow_isnt)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_isnt != NULL)
    {
        (void)(ow_isnt);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWrite(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_isnt Instance that contains One Wire parameters handlers
 * @param[in]   msg Message to write
 * @param[in]   length Number of byte to write
 * @retval      #THAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t OwWrite(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((ow_isnt != NULL) && (msg != NULL) && (length != 0u))
    {
        (void)(ow_isnt);
        (void)(msg);
        (void)(length);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwRead(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_isnt Instance that contains One Wire parameters handlers
 * @param[out]  msg Message read
 * @param[in]   length Number of byte to read
 * @retval      #THAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t OwRead(owInst_t *ow_isnt, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((ow_isnt != NULL) && (msg != NULL) && (length != 0u))
    {
        (void)(ow_isnt);
        (void)(msg);
        (void)(length);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwIoctl(owInst_t *ow_isnt, halIoCtlCmd_t io_cmd)
 * @brief           One Wire IO control function (currently used to init One Wire exchange)
 * @param[in]       ow_isnt Instance that contains One Wire parameters handlers
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwIoctl(owInst_t *ow_isnt, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_isnt != NULL)
    {
        (void)(ow_isnt);
        (void)(io_cmd);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwClose(owInst_t *ow_isnt)
 * @brief           Function that uninitialise an One Wire peripheral
 * @param[in,out]   ow_isnt Instance that contains One Wire parameters handlers
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwClose(owInst_t *ow_isnt)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_isnt != NULL)
    {
        (void)(ow_isnt);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}