import csv
import argparse
import os
from datetime import datetime

# Constants for the fixed parts of the C and header files
C_FILE_HEADER_TEMPLATE = """/**
 * @file    peripherals_conf.c
 * @brief   Source file containing peripherals information
 * @date    {date}
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"

/***************************** Macros Definitions ****************************/

#define IN_MUTEX_QUEUE_SECTION  __attribute__((section(".mutex_queues")))   /**< Mutex queue go to .mutex_queues section */

/*************************** Variables Declarations **************************/

"""

HEADER_FILE_HEADER_TEMPLATE = """/**
 * @file    peripherals_conf.h
 * @brief   Header file containing peripherals information
 * @date    {date}
 */

#ifndef PERIPHERALS_CONF_H
#define PERIPHERALS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_PERIPHERALS {nb_peripherals}u

{defines}

#endif /* PERIPHERALS_CONF_H */
"""

# Function to generate #define values for the header file
def generate_define_value(peripheral, index):
    return f"#define {peripheral.upper()} {index}u"

# Function to generate the g_peripherals_desc_table entry
def generate_desc_table_entry(peripheral, p_type):
    return f"    {{ .type = PERIPHERALS_{p_type.upper()} , .p_instance = &{peripheral.lower()}_inst }},"


# Function to generate the g_peripherals_conf_table entry
def generate_conf_table_entry(peripheral):
    return f"    {{ .p_mutex_queue = &{peripheral.lower()}_mutex_queue }},"


# Functions to generate lines for the C file
def generate_c_instance(peripheral, p_type, params):
    instance_name = f"{peripheral.lower()}_inst"
    struct_name = f"{p_type.lower()}Inst_t"
    
    params_str = "\n".join([f"    .{param} = {value}," for param, value in params.items()])
    
    return f"""
/**
 * @var     {instance_name}
 * @brief   {peripheral.lower()} instance declaration
 */
static {struct_name} {instance_name} = {{
{params_str}
}};
"""

# Function to generate mutex queue definition in the C file
def generate_mutex_queue_definition(peripheral):
    return f"""
/**
 * @var     {peripheral.lower()}_mutex_queue
 * @brief   Mutex queue for {peripheral}
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {peripheral.lower()}_mutex_queue = {{0}};
"""

# Function to generate instance and mutex queue declarations in the C file
def generate_variable_declarations(peripherals):
    instance_declarations = []
    mutex_declarations = []
    
    for peripheral, p_type in peripherals:
        instance_name = f"{peripheral.lower()}_inst"
        mutex_name = f"{peripheral.lower()}_mutex_queue"
        struct_name = f"{p_type.lower()}Inst_t"
        instance_declarations.append(f"static {struct_name} {instance_name};\n")
        mutex_declarations.append(f"static mutexQueue_t {mutex_name};\n")
    
    return instance_declarations, mutex_declarations

# Reading the CSV and generating the C and header files
def generate_peripherals_files(csv_file, output_folder):
    instances = []
    defines = []
    desc_table_entries = []
    conf_table_entries = []
    mutex_queue_definitions = []
    peripherals = []  # List to keep track of peripherals and their types
    current_date = datetime.now().strftime("%d/%m/%Y")
    
    with open(csv_file, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        
        for index, row in enumerate(reader):
            peripheral = row["Peripheral"]
            p_type = row["Peripheral Type"]
            
            peripherals.append((peripheral, p_type))
            defines.append(generate_define_value(peripheral, index))
            desc_table_entries.append(generate_desc_table_entry(peripheral, p_type))
            conf_table_entries.append(generate_conf_table_entry(peripheral))
            
            params = {}
            for key, value in row.items():
                if key.startswith("Parameter") and value:
                    param_number = key.split()[-1]
                    param = value
                    param_value_key = f"Value {param_number}"
                    param_value = row[param_value_key]
                    params[param] = param_value
            
            instances.append(generate_c_instance(peripheral, p_type, params))
            mutex_queue_definitions.append(generate_mutex_queue_definition(peripheral))
    
    # Generate variable declarations
    instance_declarations, mutex_declarations = generate_variable_declarations(peripherals)
    
    # Preparing file paths
    c_file_path = os.path.join(output_folder, "peripherals_conf.c")
    h_file_path = os.path.join(output_folder, "peripherals_conf.h")
    
    # Writing the C file
    with open(c_file_path, "w") as cfile:
        cfile.write(C_FILE_HEADER_TEMPLATE.format(date=current_date))
        
        # Write instance declarations first
        cfile.write("".join(instance_declarations))
        cfile.write("\n")
        # Then write mutex queue declarations
        cfile.write("".join(mutex_declarations))
        
        cfile.write("""
/*************************** Variables Definitions ***************************/

/**
 * @var     g_peripherals_conf_table
 * @brief   Configuration table where all peripherals configurations are stored
 */
peripheralConf_t IN_CONF_TABLES_SECTION g_peripherals_conf_table[NB_PERIPHERALS] = 
{
""")
        cfile.write("\n".join(conf_table_entries))
        cfile.write("\n};\n")
        cfile.write("""
/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
peripheralDesc_t IN_DESC_TABLES_SECTION g_peripherals_desc_table[NB_PERIPHERALS] = 
{
""")
        cfile.write("\n".join(desc_table_entries))
        cfile.write("\n};\n")

        cfile.write("".join(instances))
        cfile.write("".join(mutex_queue_definitions))

    # Writing the header file
    with open(h_file_path, "w") as hfile:
        hfile.write(HEADER_FILE_HEADER_TEMPLATE.format(date=current_date, nb_peripherals=len(peripherals), defines="\n".join(defines)))

# Argument parser configuration
parser = argparse.ArgumentParser(description='Generates peripherals_conf.c and peripherals_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, required=True, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, required=True, help='Destination folder for generated files.')

# Parsing arguments
args = parser.parse_args()

# Executing the script with provided arguments
generate_peripherals_files(args.input, args.output)
