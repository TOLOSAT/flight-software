/**
 * @file    pus160.c
 * @author  Théo Bessel
 * @brief   Source file for PUS 160 functions (System management)
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include <string.h>

#include "kernel.h"
#include "tm_management.h"
#include "services/pus160.h"
#include "system/context.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

static returnCode_t BuildS160SS18(pusTM_t *tm);
static returnCode_t BuildS160SS20(pusTM_t *tm);
static returnCode_t BuildS160SS22(pusTM_t *tm);
static returnCode_t BuildS160SS34(pusTM_t *tm);
static returnCode_t BuildS160SS36(pusTM_t *tm);
static returnCode_t BuildS160SS38(pusTM_t *tm);

/*************************** Variables Definitions ***************************/

/**
 * @var     pus160_dev_reboot
 * @brief   Device for rebooting the system
 */
static deviceNo_t pus160_dev_reboot = 0u;

/**
 * @var pus160_dev_context
 * @brief Device for reading system context
 */
static deviceNo_t pus160_dev_context = 0u;

/*************************** Functions Definitions ***************************/

/**
 * @fn          InitS160(void)
 * @brief       Function that initialises PUS 160
 * @retval      #RET_ERROR if cannot bind the pus160_dev_reboot to the reboot
 * @retval      #RET_SUCCESSFUL else
 */
returnCode_t InitS160(void)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    // Start S160 by opening a device for rebooting the system
    return_value = DeviceOpen(&pus160_dev_reboot, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_REBOOT);

    if (return_value == RET_SUCCESSFUL)
    {
        return_value = DeviceOpen(&pus160_dev_context, DEVICE_TYPE_SYSTEM, SYSDEV_SYSTEM_CONTEXT);
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS1(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    LOG("[TM/TC] TODO : S160SS1 not implemented\n");

    return return_value;
}

/**
 * @fn          ExecuteS160SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that achieve a reboot to safe mode
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for the sent TM
 */
returnCode_t ExecuteS160SS2(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    LOG("[TM/TC] Rebooting ...\n");

    return_value = DeviceIoctl(pus160_dev_reboot, 0u, NULL, 0u);

    (void)DeviceClose(pus160_dev_reboot);

    return return_value;
}

/**
 * @fn          ExecuteS160SS17(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS17(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    (void)(tc);

    returnCode_t return_value = RET_SUCCESSFUL;
    *error_code               = PUS_EXECUTION_NO_ERROR;

    return_value = BuildS160SS18(tm);

    if (return_value != RET_SUCCESSFUL)
    {
        *error_code = PUS_EXECUTION_FAILED;
    }

    return return_value;
}

/**
 * @fn          ExecuteS160SS19(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS19(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    (void)BuildS160SS20(tm);

    LOG("[TM/TC] TODO : S160SS19 not implemented\n");

    return return_value;
}

/**
 * @fn          ExecuteS160SS21(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS21(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    (void)BuildS160SS22(tm);

    LOG("[TM/TC] TODO : S160SS21 not implemented\n");

    return return_value;
}

/**
 * @fn          ExecuteS160SS33(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS33(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    (void)BuildS160SS34(tm);

    LOG("[TM/TC] TODO : S160SS33 not implemented\n");

    return return_value;
}

/**
 * @fn          ExecuteS160SS35(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS35(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    (void)(tm);
    *error_code = PUS_EXECUTION_NO_ERROR;

    (void)BuildS160SS36(tm);

    LOG("[TM/TC] TODO : S160SS35 not implemented\n");

    return return_value;
}

/**
 * @fn          ExecuteS160SS37(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
 * @brief       Function that ...
 * @param[in]   tc TC that has been received
 * @param[out]  tm TM that will be sent
 * @param[out]  error_code Indicates which error has been encountered for ... TM
 */
returnCode_t ExecuteS160SS37(pusTC_t *tc, pusTM_t *tm, pusExecutionError_t *error_code)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tc);
    *error_code = PUS_EXECUTION_NO_ERROR;

    (void)BuildS160SS38(tm);

    LOG("[TM/TC] TODO : S160SS37 not implemented\n");

    return return_value;
}

/**
 * @fn          BuildS160SS18(pusTM_t *tm)
 * @brief       Function that sends the memory context of the system
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS18(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;
    context_t context = { 0 };

    return_value = DeviceRead(pus160_dev_context, (data_t)&context, sizeof(context_t));

    if ((tm != NULL) && (return_value == RET_SUCCESSFUL))
    {
        return_value = BuildTM(tm, 160u, 18u, (pusData_t *)&context, sizeof(context_t));
    }
    else
    {
        return_value = RET_INVALID_PARAM;
    }

    (void)DeviceClose(pus160_dev_context);

    return return_value;
}

/**
 * @fn          BuildS160SS20(pusTM_t *tm)
 * @brief       Function that sends ...
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS20(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tm);

    // BuildTM(...)

    LOG("[TM/TC] TODO : S160SS20 not implemented\n");

    return return_value;
}

/**
 * @fn          BuildS160SS22(pusTM_t *tm)
 * @brief       Function that sends ...
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS22(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tm);

    // BuildTM(...)

    LOG("[TM/TC] TODO : S160SS22 not implemented\n");

    return return_value;
}

/**
 * @fn          BuildS160SS34(pusTM_t *tm)
 * @brief       Function that sends ...
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS34(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tm);

    // BuildTM(...)

    LOG("[TM/TC] TODO : S160SS34 not implemented\n");

    return return_value;
}

/**
 * @fn          BuildS160SS36(pusTM_t *tm)
 * @brief       Function that sends ...
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS36(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tm);

    // BuildTM(...)

    LOG("[TM/TC] TODO : S160SS36 not implemented\n");

    return return_value;
}

/**
 * @fn          BuildS160SS38(pusTM_t *tm)
 * @brief       Function that sends ...
 * @param[out]  tm TM that will be sent
 * @param[in]   memory_dump Data dumped that will be send
 */
static returnCode_t BuildS160SS38(pusTM_t *tm)
{
    returnCode_t return_value = RET_SUCCESSFUL;

    (void)(tm);

    // BuildTM(...)

    LOG("[TM/TC] TODO : S160SS38 not implemented\n");

    return return_value;
}