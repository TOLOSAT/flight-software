/**
 * @file dummy_tasks.h
 * @author Merlin Kooshmanian
 * @brief Header file with dummy tasks
 * @date 26/04/2023
 * 
 * Last Update : 26/04/2023
 * @copyright Copyright (c) TOLOSAT 2023
 */

#ifndef DUMMY_TASKS_H
#define DUMMY_TASKS_H

/***************************** Include Files *********************************/

#include <stdint.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

void StartBlink01(void *current_status);
void StartBlink02(void *current_status);
void DummyMainTask(void *current_status);

#endif /* DUMMY_TASKS_H */