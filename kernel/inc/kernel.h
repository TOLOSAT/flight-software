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

#include <string.h>

#include "kernel_types.h"
#include "utils/os.h"
#include "core/tasks.h"
#include "core/buffers.h"
#include "core/mutex.h"
#include "core/devices.h"
#include "core/irq.h"
#include "core/time.h"
#include "fs/fs.h"
#include "fdir/fdir.h"
#include "utils/console.h"
#include "utils/monitoring.h"
#include "utils/sys_info.h"
#include "utils/ecc.h"
#include "utils/endianness.h"
#include "drv/peripherals.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"
#include "conf/mutex_conf.h"
#include "conf/fs_conf.h"
#include "conf/peripherals_conf.h"

#endif /* KERNEL_H */

/**
 * @}
 */