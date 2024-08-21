import csv
import argparse
import os
from datetime import datetime

# Constants for the fixed parts of the C and header files
C_FILE_HEADER_TEMPLATE = """/**
 * @file    peripherals_conf.c
 * @brief   Source file containing peripherals information
 * @author  Auto-generated
 * @date    {date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CONF_TABLES_SECTION  __attribute__((section(".conf_tables")))    /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION  __attribute__((section(".desc_tables")))    /**< Descriptor table goes to .desc_tables section */

/*************************** Variables Definitions ***************************/
"""

HEADER_FILE_HEADER_TEMPLATE = """/**
 * @file    peripherals_conf.h
 * @brief   Header file containing peripherals information
 * @author  Auto-generated
 * @date    {date}
 */

#ifndef PERIPHERALS_CONF_H
#define PERIPHERALS_CONF_H

/******************************* Include Files *******************************/

#include "peripherals.h"
#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    PERIPHERALS_ENUM
 * @brief   Enum defining peripherals reference numbers
 */
enum PERIPHERALS_ENUM {{
    {enums}
    NB_PERIPHERALS
}};

/*************************** Variables Declarations **************************/

extern peripheralDesc_t g_peripherals_desc_table[NB_PERIPHERALS];

{headers}
"""

HEADER_FILE_FOOTER = """#endif /* PERIPHERALS_CONF_H */"""

# Function to generate enum values for the header file
def generate_enum_value(peripheral):
    return f"{peripheral.upper()},"

# Function to generate the g_peripherals_desc_table entry
def generate_desc_table_entry(peripheral, p_type):
    return f"    {{ .type = PERIPHERALS_{p_type.upper()} , .p_instance = &{peripheral.lower()}_inst }},"


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
{struct_name} IN_GENERIC_HAL_DATA_SECTION {instance_name} = {{
{params_str}
}};
"""

# Function to generate instance declarations in the header file
def generate_header_instance(peripheral, p_type):
    instance_name = f"{peripheral.lower()}_inst"
    struct_name = f"{p_type.lower()}Inst_t"
    return f"extern {struct_name} {instance_name};\n"

# Reading the CSV and generating the C and header files
def generate_peripherals_files(csv_file, output_folder):
    instances = []
    headers = []
    enums = []
    desc_table_entries = []
    current_date = datetime.now().strftime("%d/%m/%Y")
    
    with open(csv_file, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        
        for row in reader:
            peripheral = row["Peripheral"]
            p_type = row["Peripheral Type"]
            
            enums.append(generate_enum_value(peripheral))
            desc_table_entries.append(generate_desc_table_entry(peripheral, p_type))
            
            params = {}
            for key, value in row.items():
                if key.startswith("Parameter") and value:
                    param_number = key.split()[-1]
                    param = value
                    param_value_key = f"Value {param_number}"
                    param_value = row[param_value_key]
                    params[param] = param_value
            
            instances.append(generate_c_instance(peripheral, p_type, params))
            headers.append(generate_header_instance(peripheral, p_type))
    
    # Preparing file paths
    c_file_path = os.path.join(output_folder, "peripherals_conf.c")
    h_file_path = os.path.join(output_folder, "peripherals_conf.h")
    
    # Writing the C file
    with open(c_file_path, "w") as cfile:
        cfile.write(C_FILE_HEADER_TEMPLATE.format(date=current_date))
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

    # Writing the header file
    with open(h_file_path, "w") as hfile:
        hfile.write(HEADER_FILE_HEADER_TEMPLATE.format(date=current_date, enums="\n    ".join(enums), headers="".join(headers)))
        hfile.write(HEADER_FILE_FOOTER)

# Argument parser configuration
parser = argparse.ArgumentParser(description='Generates peripherals_conf.c and peripherals_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, required=True, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, required=True, help='Destination folder for generated files.')

# Parsing arguments
args = parser.parse_args()

# Executing the script with provided arguments
generate_peripherals_files(args.input, args.output)
