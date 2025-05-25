#!/usr/bin/env python3

import os
import json
import argparse
from datetime import datetime

# ==============================================================================
# ===================== Generation of tasks configuration ======================
# ==============================================================================

def generate_tasks_conf(tasks, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    tasks_c_filename = os.path.join(output_directory, "tasks_conf.c")
    tasks_h_filename = os.path.join(output_directory, "tasks_conf.h")

    # Extract task references and compute stack sizes
    task_refs = [task["ref"] for task in tasks]
    stack_sizes = []
    for task in tasks:
        size = task["stack_size"]
        size_str = str(size)
        if size_str.isdigit():
            size_str += "u"
        stack_sizes.append(size_str)

    # Generate stack size macros
    stack_macros = ""
    for ref, size in zip(task_refs, stack_sizes):
        task_ref_macro = ref.upper().replace(" ", "_") + "_STACK_SIZE"
        stack_macros += f"#define {task_ref_macro} {size} /**< {ref} Stack Size */\n"

    # Extract unique task functions for external declarations
    functions = {task["function"] for task in tasks}
    func_declarations = ""
    for func in functions:
        func_declarations += f"extern void {func}(void);\n"

    # Function to generate one row of the task configuration table
    def task_static_row(task):
        ref = task["ref"].upper().replace(" ", "_")
        name = task["name"].replace('"', '').strip()
        function = task["function"]
        priority = task["priority"]
        stack_size_macro = ref + "_STACK_SIZE"
        default_period = str(task["period"])
        if default_period.isdigit():
            default_period += "u"
        privilege = task["privilege"]
        stack_name = ref.lower() + "_stack"
        tcb_name = ref.lower() + "_tcb"
        return f'    {{ {ref}, "{name}", (taskFunction_t){function}, {priority}, {stack_size_macro}, {default_period}, {privilege}, &{tcb_name}, {stack_name} }},\n'

    task_config_entries = "".join(task_static_row(task) for task in tasks)

    # Generate descriptor entries (one per task)
    task_desc_entries = ""
    for ref in task_refs:
        formatted_ref = ref.upper().replace(" ", "_")
        task_desc_entries += f"    {{ .mode = TASK_NOMINAL }}, /* {formatted_ref} */\n"

    # Function to generate declarations for task stacks and TCBs (to be added in the Variables Declarations section)
    def generate_stack_and_tcb_declarations(task_refs):
        declarations = ""
        for ref in task_refs:
            formatted_ref = ref.upper().replace(" ", "_")
            stack_name = formatted_ref.lower() + "_stack"
            tcb_name = formatted_ref.lower() + "_tcb"
            declarations += f"static taskStack_t {stack_name}[{formatted_ref}_STACK_SIZE/sizeof(taskStack_t)];\n"
            declarations += f"static taskTCB_t {tcb_name};\n"
        return declarations

    # Function to generate definitions for task stacks and TCBs (to be added in the Variables Definitions section)
    def generate_stack_and_tcb_definitions(task_refs):
        definitions = ""
        for ref in task_refs:
            formatted_ref = ref.upper().replace(" ", "_")
            stack_name = formatted_ref.lower() + "_stack"
            tcb_name = formatted_ref.lower() + "_tcb"
            definitions += f"""
/**
 * @var     {stack_name}
 * @brief   Stack for {formatted_ref}
 */
static taskStack_t IN_TASK_STACKS_SECTION {stack_name}[{formatted_ref}_STACK_SIZE/sizeof(taskStack_t)] __attribute__((aligned({formatted_ref}_STACK_SIZE))) = {{0}};
"""
            definitions += f"""
/**
 * @var     {tcb_name}
 * @brief   Task Control Block for {formatted_ref}
 */
static taskTCB_t IN_TASK_TCB_SECTION {tcb_name} = {{0}};
"""
        return definitions

    stack_and_tcb_decls = generate_stack_and_tcb_declarations(task_refs)
    stack_and_tcb_defs = generate_stack_and_tcb_definitions(task_refs)

    # Construct the content of the tasks_conf.c file
    header_c = f"""/**
 * @file    tasks_conf.c
 * @brief   Source file storing the configuration table for tasks
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/tasks.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/\n
"""
    # Variables Declarations section: declare task stacks and TCBs
    vars_decls = "\n/*************************** Variables Declarations **************************/\n\n"
    vars_decls += stack_and_tcb_decls

    # Variables Definitions section: first define the configuration and descriptor tables, then the stacks and TCBs
    vars_defs = "\n/*************************** Variables Definitions **************************/\n\n"
    vars_defs += f"""/**
 * @var     g_tasks_conf_table
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskConf_t IN_CONF_TABLES_SECTION g_tasks_conf_table[CONFIG_MAX_NB_TASKS] =
{{
{task_config_entries}}};

/**
 * @var     g_tasks_desc_table
 * @brief   Descriptor table where all tasks descriptors are stored
 */
taskDesc_t IN_DESC_TABLES_SECTION g_tasks_desc_table[CONFIG_MAX_NB_TASKS] =
{{
{task_desc_entries}}};
"""
    vars_defs += stack_and_tcb_defs

    tasks_c_content = header_c + stack_macros + \
                      "\n/*************************** Functions Declarations **************************/\n\n" + func_declarations + \
                      vars_decls + vars_defs

    with open(tasks_c_filename, "w") as f:
        f.write(tasks_c_content)

    # Construct the content of the tasks_conf.h header file
    header_h = f"""/**
 * @file    tasks_conf.h
 * @brief   Header file storing the configuration table for tasks
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/***************************** Macros Definitions ****************************/\n
"""
    tasks_h_content = header_h + f"#define NB_TASKS {len(task_refs)}u\n\n"
    for idx, ref in enumerate(task_refs, start=1):
        tasks_h_content += f"#define {ref.upper().replace(' ', '_')} {idx}u\n"
    tasks_h_content += "\n#endif /* TASKS_CONF_H */\n"

    with open(tasks_h_filename, "w") as f:
        f.write(tasks_h_content)

# ==============================================================================
# ===================== Generation of buffers configuration ====================
# ==============================================================================

def generate_buffers_conf(buffers, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    buffers_c_filename = os.path.join(output_directory, "buffers_conf.c")
    buffers_h_filename = os.path.join(output_directory, "buffers_conf.h")

    buffer_defines = ""
    buffer_defs = ""
    buffer_static_conf_entries = ""
    for i, buf in enumerate(buffers, start=1):
        ref = buf["ref"]
        sender = buf["sender_ref"]
        receiver = buf["receiver_ref"]
        width = str(buf["width"])
        if width.isdigit():
            width += "u"
        depth = str(buf["depth"])
        if depth.isdigit():
            depth += "u"
        buffer_defines += f"#define {ref} {i}u\n"
        buffer_defs += f"#define {ref}_MSG_SIZE {width} /**< {ref} Message Size */\n"
        buffer_defs += f"#define {ref}_MSG_NB {depth} /**< {ref} Message Number */\n"
        buffer_static_conf_entries += f"    {{ {ref}, {sender}, {receiver}, {ref}_MSG_SIZE, {ref}_MSG_NB, &{ref.lower()}_queue, {ref.lower()}_array }},\n"

    header_h = f"""/**
 * @file    buffers_conf.h
 * @brief   Header file for buffer configuration
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_BUFFERS {len(buffers)}u

{buffer_defines}
#endif /* BUFFERS_CONF_H */
"""
    header_c = f"""/**
 * @file    buffers_conf.c
 * @brief   Source file storing configuration table for buffers
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/buffers.h"
#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

{buffer_defs}
"""
    c_content = header_c + "/*************************** Variables Declarations **************************/\n\n"
    # Static declarations for arrays and queues
    for buf in buffers:
        ref = buf["ref"]
        c_content += f"static bufferArray_t {ref.lower()}_array[{ref}_MSG_SIZE*{ref}_MSG_NB];\n"
    c_content += "\n"
    for buf in buffers:
        ref = buf["ref"]
        c_content += f"static bufferQueue_t {ref.lower()}_queue;\n"
    c_content += "\n/*************************** Variables Definitions ***************************/\n\n"
    c_content += """/**
 * @var     g_buffers_conf_table
 * @brief   Configuration table where all buffers' static parameters are stored
 */
"""
    c_content += "const bufferConf_t IN_CONF_TABLES_SECTION g_buffers_conf_table[CONFIG_MAX_NB_BUFFERS] =\n{\n" + buffer_static_conf_entries + "};\n\n"
    c_content += """/**
 * @var     g_buffers_desc_table
 * @brief   Configuration table where all buffers' descriptors are stored
 */
bufferDesc_t IN_DESC_TABLES_SECTION g_buffers_desc_table[CONFIG_MAX_NB_BUFFERS] = {0};\n"""
    # Definition of tables and tails with comments
    for buf in buffers:
        ref = buf["ref"]
        c_content += f"""
/**
 * @var     {ref.lower()}_array
 * @brief   Data array for {ref}
 */
static bufferArray_t IN_BUFFER_ARRAYS_SECTION {ref.lower()}_array[{ref}_MSG_SIZE*{ref}_MSG_NB] = {{0}};
"""
    for buf in buffers:
        ref = buf["ref"]
        c_content += f"""
/**
 * @var     {ref.lower()}_queue
 * @brief   Queue structure for {ref}
 */
static bufferQueue_t IN_BUFFER_QUEUES_SECTION {ref.lower()}_queue = {{0}};
"""
    with open(buffers_h_filename, "w") as f:
        f.write(header_h)
    with open(buffers_c_filename, "w") as f:
        f.write(c_content)

# ==============================================================================
# ===================== Generation of mutexes configuration ====================
# ==============================================================================

def generate_mutexes_conf(mutexes, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    mutex_c_filename = os.path.join(output_directory, "mutex_conf.c")
    mutex_h_filename = os.path.join(output_directory, "mutex_conf.h")

    mutex_refs = [m["ref"] for m in mutexes]
    c_content = f"""/**
 * @file    mutex_conf.c
 * @brief   Source file stocking configuration table for mutex
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/mutex.h"
#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/\n
"""
    for ref in mutex_refs:
        c_content += f"static mutexQueue_t {ref.lower()}_queue;\n"
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutexes_conf_table
 * @brief   Configuration table where all mutexes configuration are stored
 */
const mutexConf_t IN_CONF_TABLES_SECTION g_mutexes_conf_table[CONFIG_MAX_NB_MUTEXES] =
{
"""
    for ref in mutex_refs:
        c_content += f"    {{.mutex = {ref}, .p_queue = &{ref.lower()}_queue}}, /* {ref} */\n"
    c_content += "};\n\n"
    c_content += f"""/**
 * @var     g_mutexes_desc_table
 * @brief   Configuration table where all mutexes descriptors are stored
 */
mutexDesc_t IN_DESC_TABLES_SECTION g_mutexes_desc_table[CONFIG_MAX_NB_MUTEXES] = {{0}};
"""
    for ref in mutex_refs:
        c_content += f"""
/**
 * @var     {ref.lower()}_queue
 * @brief   Queue array for {ref}
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {ref.lower()}_queue = {{0}};
"""
    h_content = f"""/**
 * @file    mutex_conf.h
 * @brief   Header file stocking configuration table for mutex
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef MUTEX_CONF_H
#define MUTEX_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_MUTEXES {len(mutex_refs)}u

"""
    for idx, ref in enumerate(mutex_refs, start=1):
        h_content += f"#define {ref} {idx}u\n"
    h_content += "\n#endif /* MUTEX_CONF_H */\n"

    with open(mutex_h_filename, "w") as f:
        f.write(h_content)
    with open(mutex_c_filename, "w") as f:
        f.write(c_content)

# ==============================================================================
# ===================== Generation of files configuration ======================
# ==============================================================================

def generate_files_conf(files, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    files_c_filename = os.path.join(output_directory, "fs_conf.c")
    files_h_filename = os.path.join(output_directory, "fs_conf.h")

    file_refs = []
    file_paths = []
    file_access_modes = []
    auto_sync_modes = []
    for f_item in files:
        file_refs.append(f_item["ref"])
        file_paths.append(f_item["path"].replace('"', '').strip())
        file_access_modes.append(f_item["access"].strip())
        auto_sync_modes.append(f_item["synchronisation"].strip())

    c_content = f"""/**
 * @file    fs_conf.c
 * @brief   Source file storing configuration for file system content
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "fs/fs.h"
#include "conf/fs_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/\n
"""
    for ref in file_refs:
        temp_file_var = f"{ref.lower()}_temp_file"
        c_content += f"static FIL {temp_file_var};\n"
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_file_conf_table
 * @brief   Configuration table where all file configurations are stored
 */
fsFileConf_t IN_CONF_TABLES_SECTION g_files_conf_table[CONFIG_MAX_NB_FILES] =
{
    /* File Name, Access Mode, Auto Sync */
"""
    for ref, path, mode, auto_sync in zip(file_refs, file_paths, file_access_modes, auto_sync_modes):
        c_content += f'    {{ {ref}, "{path}", {mode}, {auto_sync} }},\n'
    c_content += "};\n"
    c_content += """
/**
 * @var     g_file_desc_table
 * @brief   Descriptor table where all file descriptors are stored
 */
fsFileDesc_t IN_DESC_TABLES_SECTION g_files_desc_table[CONFIG_MAX_NB_FILES] =
{
    /* Temp File */
"""
    for ref in file_refs:
        temp_file_var = f"{ref.lower()}_temp_file"
        c_content += f'    {{ .temp_file = &{temp_file_var} }},\n'
    c_content += "};\n"
    for ref in file_refs:
        temp_file_var = f"{ref.lower()}_temp_file"
        c_content += f"""
/**
 * @var     {temp_file_var}
 * @brief   Temporary file used for {ref}
 */
static FIL IN_TMPFS_SECTION {temp_file_var} = {{0}};
"""
    h_content = f"""/**
 * @file    fs_conf.h
 * @brief   Header file storing configuration for file system content
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef FS_CONF_H
#define FS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_FILES {len(file_refs)}u

"""
    for idx, ref in enumerate(file_refs, start=1):
        h_content += f"#define {ref} {idx}u\n"
    h_content += "\n#endif /* FS_CONF_H */\n"

    with open(files_h_filename, "w") as f:
        f.write(h_content)
    with open(files_c_filename, "w") as f:
        f.write(c_content)

# ==============================================================================
# ===================== Generation of timers configuration =====================
# ==============================================================================

def generate_timers_conf(timers, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    timers_c_filename = os.path.join(output_directory, "timers_conf.c")
    timers_h_filename = os.path.join(output_directory, "timers_conf.h")

    timer_defines = ""
    timer_static_conf_entries = ""
    for i, timer in enumerate(timers, start=1):
        ref = timer["ref"]
        owner = timer["owner"]
        timer_defines += f"#define {ref} {i}u\n"
        timer_static_conf_entries += f"    {{ {ref}, {owner} }},\n"

    h_content = f"""/**
 * @file    timers_conf.h
 * @brief   Header file for timer configuration
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef TIMERS_CONF_H
#define TIMERS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_TIMERS {len(timers)}u

{timer_defines}
#endif /* TIMERS_CONF_H */
"""
    c_content = f"""/**
 * @file    timers_conf.c
 * @brief   Source file storing configuration table for timers
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "core/timers.h"
#include "conf/timers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_timers_conf_table
 * @brief   Configuration table where all timers' static parameters are stored
 */
"""
    c_content += "const timerConf_t IN_CONF_TABLES_SECTION g_timers_conf_table[CONFIG_MAX_NB_TIMERS] =\n{\n" + timer_static_conf_entries + "};\n\n"
    c_content += """/**
 * @var     g_timers_desc_table
 * @brief   Configuration table where all timers' descriptors are stored
 */
timerDesc_t IN_DESC_TABLES_SECTION g_timers_desc_table[CONFIG_MAX_NB_TIMERS] = {0};\n"""

    with open(timers_h_filename, "w") as f:
        f.write(h_content)
    with open(timers_c_filename, "w") as f:
        f.write(c_content)

# ==============================================================================
# ==================== Generation of peripherals configuration =================
# ==============================================================================

def generate_peripherals_conf(peripherals, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    peripherals_c_filename = os.path.join(output_directory, "peripherals_conf.c")
    peripherals_h_filename = os.path.join(output_directory, "peripherals_conf.h")

    C_FILE_HEADER_TEMPLATE = f"""/**
 * @file    peripherals_conf.c
 * @brief   Source file containing peripherals information
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"
#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_MUTEX_QUEUE_SECTION  __attribute__((section(".mutex_queues")))   /**< Mutex queue go to .mutex_queues section */

/*************************** Variables Declarations **************************/\n
"""
    HEADER_FILE_HEADER_TEMPLATE = f"""/**
 * @file    peripherals_conf.h
 * @brief   Header file containing peripherals information
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef PERIPHERALS_CONF_H
#define PERIPHERALS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_PERIPHERALS {{nb_peripherals}}u

{{defines}}

#endif /* PERIPHERALS_CONF_H */
"""
    defines = []
    desc_table_entries = []
    conf_table_entries = []
    instances = []
    mutex_queue_definitions = []
    peripherals_list = []

    def generate_define_value(periph, index):
        return f"#define {periph.upper()} {index}u"
    def generate_desc_table_entry(periph):
        return f"    {{ .p_instance = &{periph.lower()}_inst }},"
    def generate_conf_table_entry(periph, p_type, p_synchro, p_flow_type):
        return (f"    {{ .type = PERIPHERAL_{p_type.upper()}, .synchronisation = PERIPHERAL_{p_synchro.upper()}, "
                f".flow_type = PERIPHERAL_{p_flow_type.upper()}, .p_mutex_queue = &{periph.lower()}_mutex_queue, "
                f".p_rx_mutex_queue = &{periph.lower()}_rx_mutex_queue, .p_tx_mutex_queue = &{periph.lower()}_tx_mutex_queue }},")
    def generate_c_instance(periph, p_type, params):
        instance_name = f"{periph.lower()}_inst"
        struct_name = f"{p_type.lower()}Inst_t"
        params_str = "\n".join([f"    .{param} = {value}," for param, value in params.items()])
        return f"""
/**
 * @var     {instance_name}
 * @brief   {periph.lower()} instance declaration
 */
static {struct_name} {instance_name} = {{
{params_str}
}};
"""
    def generate_mutex_queue_definition(periph):
        return f"""
/**
 * @var     {periph.lower()}_mutex_queue
 * @brief   Mutex queue for {periph}
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_mutex_queue = {{0}};

/**
 * @var     {periph.lower()}_rx_mutex_queue
 * @brief   Mutex queue for {periph} reception
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_rx_mutex_queue = {{0}};

/**
 * @var     {periph.lower()}_tx_mutex_queue
 * @brief   Mutex queue for {periph} transmission
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_tx_mutex_queue = {{0}};
"""
    def generate_variable_declarations(peripherals_info):
        instance_declarations = []
        mutex_declarations = []
        for periph, p_type in peripherals_info:
            instance_name = f"{periph.lower()}_inst"
            struct_name = f"{p_type.lower()}Inst_t"
            instance_declarations.append(f"static {struct_name} {instance_name};\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_mutex_queue;\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_rx_mutex_queue;\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_tx_mutex_queue;\n")
        return instance_declarations, mutex_declarations

    for index, periph in enumerate(peripherals):
        ref = periph["ref"]
        p_type = periph["type"]
        p_synchro = periph["synchronisation"]
        p_flow_type = periph["flow_type"]
        defines.append(generate_define_value(ref, index))
        desc_table_entries.append(generate_desc_table_entry(ref))
        conf_table_entries.append(generate_conf_table_entry(ref, p_type, p_synchro, p_flow_type))
        # For additional parameters, we take all the keys other than ref,type,mode,flow
        params = {}
        for key, value in periph.items():
            if key not in ["ref", "type", "synchronisation", "flow_type"]:
                params[key] = value
        instances.append(generate_c_instance(ref, p_type, params))
        mutex_queue_definitions.append(generate_mutex_queue_definition(ref))
        peripherals_list.append((ref, p_type))
    instance_declarations, mutex_declarations = generate_variable_declarations(peripherals_list)

    c_content = C_FILE_HEADER_TEMPLATE
    c_content += "".join(instance_declarations) + "\n"
    c_content += "".join(mutex_declarations) + "\n"
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_peripherals_conf_table
 * @brief   Configuration table where all peripherals configurations are stored
 */
peripheralConf_t IN_CONF_TABLES_SECTION g_peripherals_conf_table[CONFIG_MAX_NB_PERIPHERALS] =
{
"""
    c_content += "\n".join(conf_table_entries)
    c_content += "\n};\n"
    c_content += """
/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
peripheralDesc_t IN_DESC_TABLES_SECTION g_peripherals_desc_table[CONFIG_MAX_NB_PERIPHERALS] =
{
"""
    c_content += "\n".join(desc_table_entries)
    c_content += "\n};\n"
    c_content += "".join(instances)
    c_content += "".join(mutex_queue_definitions)

    h_content = HEADER_FILE_HEADER_TEMPLATE.replace("{nb_peripherals}", str(len(peripherals)))
    h_content = h_content.replace("{defines}", "\n".join(defines))

    with open(peripherals_c_filename, "w") as f:
        f.write(c_content)
    with open(peripherals_h_filename, "w") as f:
        f.write(h_content)

# ==============================================================================
# ==================== Generation of housekeeping configuration ================
# ==============================================================================

def generate_housekeeping_conf(hk_list, output_directory):
    current_date = datetime.now().strftime("%Y/%m/%d")
    hk_c_filename = os.path.join(output_directory, "hk_conf.c")
    hk_h_filename = os.path.join(output_directory, "hk_conf.h")

    # Extraction of fields from JSON
    hk_refs = [(hk["hk_ref"], hk["hkid"], hk["status"]) for hk in hk_list]

    c_content = f"""/**
 * @file    hk_conf.c
 * @brief   Source file stocking configuration table for housekeeping parameters
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "system/housekeeping.h"
#include "conf/hk_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Definitions ***************************/

/**
 * @var     g_hk_desc_table
 * @brief   Configuration table where all housekeeping parameters are stored
 */
hkDesc_t IN_DESC_TABLES_SECTION g_hk_desc_table[CONFIG_MAX_NB_HKS] =
{{
"""
    for ref, hkid, status in hk_refs:
        c_content += f'    {{ .hkid={ref}, .hk_status={status} }},\n'
    c_content += "};\n"

    h_content = f"""/**
 * @file    hk_conf.h
 * @brief   Header file stocking configuration table for housekeeping parameters
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef HK_CONF_H
#define HK_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_HKS {len(hk_refs)}u

"""
    for ref, hkid, _ in hk_refs:
        h_content += f"#define {ref} {hkid}u\n"
    h_content += "\n#endif /* HK_CONF_H */\n"

    with open(hk_c_filename, "w") as f:
        f.write(c_content)
    with open(hk_h_filename, "w") as f:
        f.write(h_content)

# ==============================================================================
# ================================ Main Function ===============================
# ==============================================================================
def main():
    parser = argparse.ArgumentParser(
        description="Génère les fichiers de configuration C/H du système embarqué à partir d'un fichier JSON unique."
    )
    parser.add_argument("-i", "--input", required=True, help="Chemin vers le fichier JSON d'entrée")
    parser.add_argument("-o", "--output", required=True, help="Dossier de destination des fichiers générés")
    args = parser.parse_args()

    if not os.path.exists(args.output):
        os.makedirs(args.output)

    with open(args.input, "r") as f:
        data = json.load(f)

    system = data.get("system", {})

    if "tasks" in system:
        generate_tasks_conf(system["tasks"], args.output)
    if "buffers" in system:
        generate_buffers_conf(system["buffers"], args.output)
    if "mutexes" in system:
        generate_mutexes_conf(system["mutexes"], args.output)
    if "files" in system:
        generate_files_conf(system["files"], args.output)
    if "timers" in system:
        generate_timers_conf(system["timers"], args.output)
    if "peripherals" in system:
        generate_peripherals_conf(system["peripherals"], args.output)
    if "housekeeping" in system:
        generate_housekeeping_conf(system["housekeeping"], args.output)

if __name__ == "__main__":
    main()
