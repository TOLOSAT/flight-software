/**
 * @file    core.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all core headers
 * @date    25/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef CORE_H
#define CORE_H

/******************************* Include Files *******************************/

#include "core_types.h"
#include "os.h"
#include "tasks.h"
#include "buffers.h"
#include "mutex.h"
#include "fs.h"
#include "time.h"
#include "fdir.h"
#include "console.h"
#include "sys_info.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"
#include "conf/fs_conf.h"
#include "conf/platform_conf.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_CORE_TEXT_SECTION    __attribute__((section(".text_core")))  /**< CORE functions goes in the .text_core */
#define IN_CORE_DATA_SECTION    __attribute__((section(".data_core")))  /**< CORE data goes in the .data_core */

#endif /* CORE_H */