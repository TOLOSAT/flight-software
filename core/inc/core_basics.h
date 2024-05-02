/**
 * @file    core_basics.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all core basics headers
 * @date    25/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef CORE_BASICS_H
#define CORE_BASICS_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "os.h"
#include "tasks.h"
#include "buffers.h"
#include "mutex.h"
#include "fdir.h"
#include "console.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CORE_TEXT_SECTION    __attribute__((section(".text.core")))  /**< CORE functions goes in the .text.core */
#define IN_CORE_DATA_SECTION    __attribute__((section(".data.core")))  /**< CORE data goes in the .data.core */

#endif /* CORE_BASICS_H */