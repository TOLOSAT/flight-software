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
        return f'    {{ .task = {ref}, .name = "{name}", .function = (taskFunction_t){function}, .priority = {priority}, .stack_size = {stack_size_macro}, .default_period = {default_period}, .privilege = {privilege} }},\n'

    task_config_entries = "".join(task_static_row(task) for task in tasks)
    task_config_entries += f"    {{ 0 }}"

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

#include "kernel_types.h"
#include "tasks_conf.h"

/***************************** Macros Definitions ****************************/\n
"""
    # Variables Declarations section: declare task stacks and TCBs
    vars_decls = "\n/*************************** Variables Declarations **************************/\n"

    # Variables Definitions section: first define the configuration and descriptor tables, then the stacks and TCBs
    vars_defs = "\n/*************************** Variables Definitions **************************/\n\n"
    vars_defs += f"""/**
 * @var     g_tasks_conf_table
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskConf_t IN_CONFIG_SECTION g_tasks_conf_table[] =
{{\n{task_config_entries}\n}};
"""
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
        buffer_static_conf_entries += f"    {{ .buffer = {ref}, .sender = {sender}, .receiver = {receiver}, .max_size = {ref}_MSG_SIZE, .max_nb = {ref}_MSG_NB }},\n"
    buffer_static_conf_entries += f"    {{ 0 }}"

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

#include "kernel_types.h"
#include "buffers_conf.h"
#include "tasks_conf.h"

/***************************** Macros Definitions ****************************/

{buffer_defs}
"""
    c_content = header_c + "/*************************** Variables Declarations **************************/\n"
    c_content += "\n/*************************** Variables Definitions ***************************/\n\n"
    c_content += f"""/**
 * @var     g_buffers_conf_table
 * @brief   Configuration table where all buffers' static parameters are stored
 */
const bufferConf_t IN_CONFIG_SECTION g_buffers_conf_table[] =
{{\n{buffer_static_conf_entries}\n}};
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

#include "kernel_types.h"
#include "mutex_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/\n"""
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutexes_conf_table
 * @brief   Configuration table where all mutexes configuration are stored
 */
const mutexConf_t IN_CONFIG_SECTION g_mutexes_conf_table[] =
{
"""
    for ref in mutex_refs:
        c_content += f"    {{ .mutex = {ref} }},\n"
    c_content += "    { 0 }"
    c_content += "\n};\n"

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
    for f_item in files:
        file_refs.append(f_item["ref"])
        file_paths.append(f_item["path"].replace('"', '').strip())
        file_access_modes.append(f_item["access_mode"].strip())

    c_content = f"""/**
 * @file    fs_conf.c
 * @brief   Source file storing configuration for file system content
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "fs_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/
"""
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_file_conf_table
 * @brief   Configuration table where all file configurations are stored
 */
const fsFileConf_t IN_CONFIG_SECTION g_files_conf_table[] =
{
    /* File Name, Access Mode */
"""
    for ref, path, mode in zip(file_refs, file_paths, file_access_modes):
        c_content += f'    {{ .file = {ref}, .name = "{path}", .access_mode = {mode} }},\n'
    c_content += "    { 0 }"
    c_content += "\n};\n"
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

    # Prepare the enum defines
    timer_defines = ""
    for i, timer in enumerate(timers, start=1):
        ref = timer["ref"]
        timer_defines += f"#define {ref} {i}u\n"

    conf_entries = ""
    for timer in timers:
        ref = timer["ref"]
        owner = timer["owner"]
        conf_entries += (
            f"    {{ .timer = {ref}, "
            f".owner = {owner} }},\n"
        )
    conf_entries += f"    {{ 0 }}"

    # --- Construct the .c file ---
    c_content = f"""/**
 * @file    timers_conf.c
 * @brief   Source file storing configuration table for timers
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "kernel_types.h"
#include "timers_conf.h"
#include "tasks_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/

/*************************** Variables Definitions **************************/

/**
 * @var     g_timers_conf_table
 * @brief   Configuration table where all timers' static parameters are stored
 */
const timerConf_t IN_CONFIG_SECTION g_timers_conf_table[] =
{{\n{conf_entries}\n}};
"""

    with open(timers_c_filename, "w") as f:
        f.write(c_content)

    # --- Construct the .h file (inchangé) ---
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
    with open(timers_h_filename, "w") as f:
        f.write(h_content)

# ==============================================================================
# ================================ Main Function ===============================
# ==============================================================================

def generate_system_conf_header(output_directory):
    """Génère un header agrégateur conf/system_conf.h qui inclut toutes les confs."""
    current_date = datetime.now().strftime("%d/%m/%Y")
    system_h_filename = os.path.join(output_directory, "system_conf.h")

    content = f"""/**
 * @file    system_conf.h
 * @brief   Header agrégateur pour inclure toutes les configurations du système
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef SYSTEM_CONF_H
#define SYSTEM_CONF_H

/******************************* Include Files *******************************/

#include "buffers_conf.h"
#include "tasks_conf.h"
#include "mutex_conf.h"
#include "fs_conf.h"
#include "timers_conf.h"

#endif /* SYSTEM_CONF_H */
"""
    with open(system_h_filename, "w") as f:
        f.write(content)

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

    # Génère le header agrégateur de conf
    generate_system_conf_header(args.output)

if __name__ == "__main__":
    main()
