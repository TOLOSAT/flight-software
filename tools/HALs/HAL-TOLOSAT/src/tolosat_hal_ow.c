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

static halStatus_t OwWriteByte(owInst_t *ow_inst, uint8_t byte);
static halStatus_t OwReadByte(owInst_t *ow_inst, uint8_t *byte);
static halStatus_t OwInitConnection(owInst_t *ow_inst);
static halStatus_t OwWriteBit(owInst_t *ow_inst, uint8_t bit);
static halStatus_t OwReadBit(owInst_t *ow_inst, uint8_t *bit);

/*************************** Variables Definitions ***************************/

/*************************** Functions Definitions ***************************/

/**
 * @fn              OwOpen(owInst_t *ow_inst)
 * @brief           Function that initialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_ERROR if an error occured
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwOpen(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioOpen(&ow_inst->gpio_handle_struct);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that writes a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   msg Message to write
 * @param[in]   length Number of byte to write
 * @retval      #THAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t OwWrite(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == THAL_SUCCESSFUL) && (i < length))
        {
            return_value = OwWriteByte(ow_inst, msg[i]);
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
 * @brief       Function that reads a message onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[out]  msg Message read
 * @param[in]   length Number of byte to read
 * @retval      #THAL_INVALID_PARAM if there is a null pointer or length is zero
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
halStatus_t OwRead(owInst_t *ow_inst, owMsg_t *msg, owMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if ((ow_inst != NULL) && (msg != NULL) && (length != 0u))
    {
        uint32_t i = 0u;
        while ((return_value == THAL_SUCCESSFUL) && (i < length))
        {
            return_value = OwReadByte(ow_inst, &msg[i]);
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
 * @brief           One Wire IO control function (currently used to init One Wire connection)
 * @param[in]       ow_inst Instance that contains One Wire parameters handlers
 * @param[in,out]   io_cmd IO Control command struct (including data)
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwIoctl(owInst_t *ow_inst, halIoCtlCmd_t io_cmd)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        switch (io_cmd.cmd)
        {
        case OW_IOCTL_INIT_CONNECTION:
            return_value = OwInitConnection(ow_inst);
            break;
        default:
            return_value = THAL_INVALID_PARAM;
            break;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwClose(owInst_t *ow_inst)
 * @brief           Function that uninitialises an One Wire peripheral
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_ERROR if an error occured
 * @retval          #THAL_SUCCESSFUL else
 */
halStatus_t OwClose(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        return_value = GpioClose(&ow_inst->gpio_handle_struct);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that writes a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to write
 * @retval      #THAL_INVALID_PARAM if there is a null pointer
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
static halStatus_t OwWriteByte(owInst_t *ow_inst, uint8_t byte)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == THAL_SUCCESSFUL) && (i < sizeof(uint8_t)))
        {

            uint8_t bit = (byte & (0x01 << i)) >> i;
            return_value = OwWriteBit(ow_inst, bit);
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadByte(owInst_t *ow_inst, uint8_t byte)
 * @brief       Function that reads a byte onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   byte Byte to read
 * @retval      #THAL_INVALID_PARAM if there is a null pointer
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
static halStatus_t OwReadByte(owInst_t *ow_inst, uint8_t *byte)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        uint32_t i = 0u;
        while ((return_value == THAL_SUCCESSFUL) && (i < sizeof(uint8_t)))
        {
            uint8_t bit = 0u;
            return_value = OwReadBit(ow_inst, &bit);
            *byte |= bit << i;
        }
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn              OwInitConnection(owInst_t *ow_inst)
 * @brief           Function that initialize a One Wire connection 
 * @param[in,out]   ow_inst Instance that contains One Wire parameters handlers
 * @retval          #THAL_INVALID_PARAM if ow_inst is a null pointer
 * @retval          #THAL_ERROR if an error occured when using GPIO
 * @retval          #THAL_SUCCESSFUL else
 */
static halStatus_t OwInitConnection(owInst_t *ow_inst)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        (void)(ow_inst);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwWriteBit(owInst_t *ow_inst, uint8_t bit)
 * @brief       Function that writes a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to write
 * @retval      #THAL_INVALID_PARAM if there is a null pointer
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
static halStatus_t OwWriteBit(owInst_t *ow_inst, uint8_t bit)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        (void)(ow_inst);
        (void)(bit);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}

/**
 * @fn          OwReadBit(owInst_t *ow_inst, uint8_t *bit)
 * @brief       Function that reads a bit onto One Wire
 * @param[in]   ow_inst Instance that contains One Wire parameters handlers
 * @param[in]   bit Bit to read
 * @retval      #THAL_INVALID_PARAM if there is a null pointer
 * @retval      #THAL_ERROR if an error occured when using GPIO
 * @retval      #THAL_SUCCESSFUL else
 */
static halStatus_t OwReadBit(owInst_t *ow_inst, uint8_t *bit)
{
    // Variable Initialisation
    halStatus_t return_value = THAL_SUCCESSFUL;

    // Function Core
    if (ow_inst != NULL)
    {
        (void)(ow_inst);
        (void)(bit);
    }
    else
    {
        return_value = THAL_INVALID_PARAM;
    }

    return return_value;
}