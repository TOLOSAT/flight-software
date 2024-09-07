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
#include "utils/os.h"
#include "base/tasks.h"
#include "base/buffers.h"
#include "base/mutex.h"
#include "fs/fs.h"
#include "time/time.h"
#include "fdir/fdir.h"
#include "utils/console.h"
#include "utils/sys_info.h"
#include "drv/peripherals.h"
#include "drv/devices.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"
#include "conf/fs_conf.h"
#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_CORE_TEXT_SECTION        __attribute__((section(".text_core")))          /**< CORE functions goes in the .text_core */
#define IN_CORE_DATA_SECTION        __attribute__((section(".data_core")))          /**< CORE data goes in the .data_core */

// Table placement
#define IN_CONF_TABLES_SECTION      __attribute__((section(".conf_tables")))        /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION      __attribute__((section(".desc_tables")))        /**< Descriptor table goes to .desc_tables section */

// Other specific sections
#define IN_DMABUFF_SECTION          __attribute__((section(".dmabuff")))            /**< Temporary file goes to .dmabuff section */
#define IN_TMPFS_SECTION            __attribute__((section(".tmpfs")))              /**< Temporary file goes to .tmpfs section */
#define IN_TASK_STACKS_SECTION      __attribute__((section(".task_stacks")))        /**< Task stacks go to .task_stacks section */
#define IN_TASK_TCB_SECTION         __attribute__((section(".task_tcbs")))          /**< Task control block go to .task_tcbs section */
#define IN_BUFFER_ARRAYS_SECTION    __attribute__((section(".buffer_arrays")))      /**< Buffer data go to .buffer_arrays section */
#define IN_BUFFER_ENTITIES_SECTION  __attribute__((section(".buffer_entities")))    /**< Buffer data go to .buffer_entities section */
#define IN_MUTEX_QUEUE_SECTION      __attribute__((section(".mutex_queues")))       /**< Mutex queue go to .mutex_queues section */

#endif /* CORE_H */