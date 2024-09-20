import csv
from datetime import datetime
import argparse
import os

# Argument parser configuration
parser = argparse.ArgumentParser(description='Generates tasks_conf.c and tasks_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, help='Destination folder for generated files.')

# Parse arguments
args = parser.parse_args()

if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Check if the output directory exists, if not create it
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, 'tasks_conf.c')
h_file_name = os.path.join(output_directory, 'tasks_conf.h')

# Get the current date for the header
current_date = datetime.now().strftime("%d/%m/%Y")

unique_includes = set()

# Helper function to generate stack macros
def generate_stack_macros(task_refs, stack_sizes):
    macros = "\n"
    for ref, size in zip(task_refs, stack_sizes):
        size += "u" if size.isdigit() else ""
        task_ref_macro = f"{ref.upper().replace(' ', '_')}_STACK_SIZE"
        macros += f"#define {task_ref_macro} {size} /**< {ref} Stack Size */\n"
    return macros

# Helper function to convert CSV rows to C static rows
def csv_to_c_static_row(row):
    task_ref = row["Task Ref"].upper().replace(' ', '_')
    name = row["Name"].replace('"', '').strip()
    function = row["Function"]
    priority = row["Priority"]
    stack_size_macro = f"{task_ref}_STACK_SIZE"
    default_period = row["Default Period"]
    default_period += "u" if default_period.isdigit() else ""
    privilege = row["Privilege"]
    memory_regions = ", ".join([x for x in row.values()][9:])
    stack_name = f"{task_ref.lower()}_stack"
    tcb_name = f"{task_ref.lower()}_tcb"
    return f'    {{ {task_ref}, "{name}", {function}, {priority}, {stack_size_macro}, {default_period}, {privilege}, {{{memory_regions}}}, &{tcb_name}, {stack_name} }},\n'

# Helper function to generate stack and TCB definitions
def generate_stack_definitions(task_refs):
    stack_definitions = ""
    tcb_definitions = ""
    for ref in task_refs:
        formatted_ref = ref.upper().replace(' ', '_')
        stack_name = f"{formatted_ref.lower()}_stack"
        tcb_name = f"{formatted_ref.lower()}_tcb"
        stack_definitions += f"""
/**
 * @var     {stack_name}
 * @brief   Stack for {formatted_ref}
 */
static taskStack_t IN_TASK_STACKS_SECTION {stack_name}[{formatted_ref}_STACK_SIZE/sizeof(taskStack_t)] __attribute__((aligned({formatted_ref}_STACK_SIZE))) = {{0}};
"""
        tcb_definitions += f"""
/**
 * @var     {tcb_name}
 * @brief   Task Control Block for {formatted_ref}
 */
static taskTCB_t IN_TASK_TCB_SECTION {tcb_name} = {{0}};
"""
    return stack_definitions + tcb_definitions

# Helper function to generate dynamic configuration
def generate_dynamic_conf(task_refs):
    dynamic_conf = """
/**
 * @var     g_tasks_desc_table
 * @brief   Configuration table where all tasks descriptors are stored
 */
taskDesc_t IN_DESC_TABLES_SECTION g_tasks_desc_table[NB_TASKS] = 
{
"""
    for ref in task_refs:
        formatted_ref = ref.upper().replace(' ', '_')
        dynamic_conf += f"    {{ .mode = TASK_NOMINAL }}, /* {formatted_ref} */\n"
    dynamic_conf += "};\n"
    return dynamic_conf

try:
    task_refs, task_names, stack_sizes = [], [], []

    # Read the CSV file
    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            task_refs.append(row["Task Ref"])
            task_names.append(row["Name"].replace('"', '').strip())
            stack_sizes.append(row["Stack Size"])
            if row.get("Include"):
                unique_includes.add(row["Include"].strip())

    includes_str = "\n".join([f'#include "{inc}"' for inc in sorted(unique_includes)])

    header_c = f"""/**
 * @file    tasks_conf.c
 * @brief   Source file storing configuration table for tasks
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/tasks_conf.h"
{includes_str}

/***************************** Macros Definitions ****************************/
"""

    # Add stack size macros to the .c file
    stack_macros = generate_stack_macros(task_refs, stack_sizes)
    header_c += stack_macros

    # Add a new section for Variable Declarations in .c
    header_c += """
/*************************** Variables Declarations **************************/

"""
    dynamic_conf = generate_dynamic_conf(task_refs)
    stack_definitions = generate_stack_definitions(task_refs)

    header_h = f"""/**
 * @file    tasks_conf.h
 * @brief   Header file storing configuration table for tasks
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/******************************* Include Files *******************************/

#include "core/tasks.h"

/***************************** Macros Definitions ****************************/
"""

    # Write the .h file
    with open(h_file_name, 'w') as h_file:
        h_file.write(header_h)
        h_file.write("""
/***************************** Types Definitions *****************************/

/**
 * @enum    TASKS_ENUM
 * @brief   Enum defining tasks reference numbers
 */
enum TASKS_ENUM {
""")
        for ref in task_refs:
            h_file.write(f"    {ref.upper().replace(' ', '_')},\n")
        h_file.write("    NB_TASKS\n};\n\n")
        h_file.write("/*************************** Variables Declarations **************************/\n\n")
        h_file.write("extern const taskConf_t g_tasks_conf[NB_TASKS];\n")
        h_file.write("extern taskDesc_t g_tasks_desc_table[NB_TASKS];\n\n")
        h_file.write("#endif /* TASKS_CONF_H */\n")

    # Write the .c file
    with open(c_file_name, 'w') as c_file:
        c_file.write(header_c)
        
        # Add stack and TCB declarations to the .c file in the Variable Declarations section
        for ref in task_refs:
            formatted_ref = ref.upper().replace(' ', '_')
            c_file.write(f"static taskStack_t {ref.lower()}_stack[{formatted_ref}_STACK_SIZE/sizeof(taskStack_t)];\n")
        c_file.write(f"\n")
        for ref in task_refs:
            c_file.write(f"static taskTCB_t {ref.lower()}_tcb;\n")
        
        c_file.write("\n/*************************** Variables Definitions ***************************/\n\n")

        # Add the missing comment for the task configuration table
        c_file.write(f"""/**
 * @var     g_tasks_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
""")

        # Write the task configuration table
        c_file.write("const taskConf_t IN_CONF_TABLES_SECTION g_tasks_conf[NB_TASKS] = \n{\n")
        for row in csv.DictReader(open(csv_file_name, mode='r', newline='')):
            c_file.write(csv_to_c_static_row(row))
        c_file.write("};\n")

        # Write dynamic configuration and stack definitions
        c_file.write(dynamic_conf)
        c_file.write(stack_definitions)

except Exception as e:
    print(f"Error when generating: {e}")
