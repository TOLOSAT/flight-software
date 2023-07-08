/**
 * @file pus1.c
 * @author Merlin Kooshmanian
 * @brief Source file for PUS 1 functions
 * @date 06/07/2023
 *
 * Last Update : 06/07/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

/***************************** Include Files *********************************/

#include <string.h>

#include "services/pus1.h"
#include "pus_tools/tm_management.h"

/************************** Constant Definitions *****************************/

#define S1SS1_DATA_SIZE 4u  /**< Size of PUS S1SS1 data field */
#define S1SS2_DATA_SIZE 5u  /**< Size of PUS S1SS1 data field */

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/************************* Functions Definitions *****************************/

/**
 * @brief Function that send S1SS1 TM (acceptance acknowledgment)
 */
pusStatus_t SendS1SS1(pusTC_t *tc)
{
    // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusTM_t tm = {0};
    pusData_t data[S1SS1_DATA_SIZE];

    // Function Core
    memcpy(&data, tc, S1SS1_DATA_SIZE);
    BuildTM(&tm, 1u, 1u, &data, S1SS1_DATA_SIZE);

    return (return_value);
}

/**
 * @brief Function that send S1SS1 TM (acceptance acknowledgment)
 */
pusStatus_t SendS1SS2(pusTC_t *tc, pusAcceptanceError_t acceptance_error)
{
        // Variable Initialisation
    pusStatus_t return_value = PUS_SUCCESSFUL;
    pusTM_t tm = {0};
    pusData_t data[S1SS2_DATA_SIZE];

    // Function Core
    memcpy(&data, tc, S1SS2_DATA_SIZE - 1u);
    data[S1SS2_DATA_SIZE - 1u] = acceptance_error;
    BuildTM(&tm, 1u, 1u, &data, S1SS1_DATA_SIZE);

    return (return_value);
}
