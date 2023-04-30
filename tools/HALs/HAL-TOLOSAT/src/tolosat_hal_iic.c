/**
 * @file tolosat_hal_iic.c
 * @author Merlin Kooshmanian
 * @brief Source file for TOLOSAT HAL IIC functions
 * @date 30/04/2023
 *
 * Last Update : 30/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include "tolosat_hal.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

halStatus_t IicOpen(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(iic_inst != NULL)
    {

    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

halStatus_t IicWrite(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {

    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

halStatus_t IicRead(iicInst_t *iic_inst, iicSlaveAddr_t slave_addr, iicMsg_t *msg, iicMsgLength_t length)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(iic_inst != NULL && msg != NULL && slave_addr != 0 && length != 0)
    {

    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

halStatus_t IictIoctl(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(iic_inst != NULL)
    {

    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}

halStatus_t IicClose(iicInst_t *iic_inst)
{
    // Variable Initialisation
    halStatus_t return_value = FCT_SUCCESSFUL;

    // Function Core
    if(iic_inst != NULL)
    {

    }
    else
    {
        return_value = FCT_INVALID_PARAM;
    }

    return(return_value);
}
