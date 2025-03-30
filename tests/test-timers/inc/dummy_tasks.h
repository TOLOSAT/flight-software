/**
<<<<<<<< HEAD:tests/test-timers/inc/dummy_tasks.h
 * @file    dummy_tasks.h
 * @author  Merlin Kooshmanian
 * @brief   Header file with dummy tasks
========
 * @file    syswdg.h
 * @author  Merlin Kooshmanian
 * @brief   Header file for system watchdog handling
>>>>>>>> main:kernel/inc/system/syswdg.h
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

<<<<<<<< HEAD:tests/test-timers/inc/dummy_tasks.h
#ifndef DUMMY_TASKS_H
#define DUMMY_TASKS_H

/******************************* Include Files *******************************/

#include <stdint.h>
========
/**
 * @defgroup kernel Kernel
 * @{
 * @defgroup system System
 * @{
 * @defgroup syswdg System Watchdog
 * @brief Provides system watchdog handling interface.
 * @{
 */

#ifndef SYSWDG_H
#define SYSWDG_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
>>>>>>>> main:kernel/inc/system/syswdg.h

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

/*************************** Functions Declarations **************************/

<<<<<<<< HEAD:tests/test-timers/inc/dummy_tasks.h
extern void DummyMainTask(void);

#endif /* DUMMY_TASKS_H */
========
extern void InitSYSWDG(void);
extern void SYSWDGMain(void);

#endif /* SYSWDG_H */

/**
 * @}
 * @}
 * @}
 */
>>>>>>>> main:kernel/inc/system/syswdg.h
