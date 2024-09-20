/**
 * @file    kernel.h
 * @author  Merlin Kooshmanian
 * @brief   Header including all kernel headers
 * @date    25/02/2024
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/**
 * @defgroup kernel Kernel
 * @{
 */

#ifndef KERNEL_H
#define KERNEL_H

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "utils/os.h"
#include "core/tasks.h"
#include "core/buffers.h"
#include "core/mutex.h"
#include "fs/fs.h"
#include "core/time.h"
#include "fdir/fdir.h"
#include "utils/console.h"
#include "utils/sys_info.h"
#include "utils/ecc.h"
#include "drv/peripherals.h"
#include "drv/devices.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"
#include "conf/fs_conf.h"
#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

// Section placement macros
#define IN_KERNEL_TEXT_SECTION        __attribute__((section(".text_kernel")))          /**< KERNEL functions goes in the .text_kernel */
#define IN_KERNEL_DATA_SECTION        __attribute__((section(".data_kernel")))          /**< KERNEL data goes in the .data_kernel */

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

#endif /* KERNEL_H */

/**
 * @}
 */