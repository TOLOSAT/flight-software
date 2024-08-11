import csv
import argparse
import os
from datetime import datetime

# Constants for the fixed parts of the C and header files
C_FILE_HEADER_TEMPLATE = """/**
 * @file    platform_conf.c
 * @brief   Source file containing platform information
 * @author  Auto-generated
 * @date    {date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/platform_conf.h"
#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Functions Declarations **************************/

/*************************** Variables Definitions ***************************/
"""

C_FILE_FOOTER = """\n/*************************** Functions Definitions ***************************/

/**
 * @fn      PlatformInit(void)
 * @brief   Function that initializes the platform
 * @retval  0 always (errors will be catch by the error handler)
 */
uint32_t IN_GENERIC_HAL_TEXT_SECTION PlatformInit(void)
{
    // Variable Initialization
    uint32_t status = 0u;

    // Initialise Peripherals
"""

C_FILE_INIT_CALLS = """

    return status;
}
"""

HEADER_FILE_HEADER_TEMPLATE = """/**
 * @file    platform_conf.h
 * @brief   Header file containing platform information
 * @author  Auto-generated
 * @date    {date}
 */

#ifndef IO_INSTANCES_H
#define IO_INSTANCES_H

/******************************* Include Files *******************************/

#include <stdint.h>

#include "generic_hal.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/*************************** Variables Declarations **************************/

"""

HEADER_FILE_FOOTER = """
/*************************** Functions Declarations **************************/

extern uint32_t PlatformInit(void);

#endif /* IO_INSTANCES_H */
"""

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

# Function to generate the PlatformInit initialization calls
def generate_init_call(peripheral, p_type):
    instance_name = f"{peripheral.lower()}_inst"
    if p_type.lower() == "gpio":
        return f"    status = GpioOpen(&{instance_name});\n    CheckErrors(status, FDIR_ERROR_HANDLER);"
    elif p_type.lower() == "uart":
        return f"    status = UartOpen(&{instance_name});\n    CheckErrors(status, FDIR_ERROR_HANDLER);"
    elif p_type.lower() == "i2c":
        return f"    status = I2cOpen(&{instance_name});\n    CheckErrors(status, FDIR_ERROR_HANDLER);"
    elif p_type.lower() == "ow":
        return f"    status = OwOpen(&{instance_name});\n    CheckErrors(status, FDIR_ERROR_HANDLER);"
    else:
        return f"    // Unsupported peripheral type: {p_type}\n"

# Reading the CSV and generating the C and header files
def generate_platform_files(csv_file, output_folder):
    instances = []
    headers = []
    init_calls = []
    current_date = datetime.now().strftime("%d/%m/%Y")
    
    with open(csv_file, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        
        for row in reader:
            peripheral = row["Peripheral"]
            p_type = row["Peripheral Type"]
            
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
            init_calls.append(generate_init_call(peripheral, p_type))
    
    # Preparing file paths
    c_file_path = os.path.join(output_folder, "platform_conf.c")
    h_file_path = os.path.join(output_folder, "platform_conf.h")
    
    # Writing the C file
    with open(c_file_path, "w") as cfile:
        cfile.write(C_FILE_HEADER_TEMPLATE.format(date=current_date))
        cfile.write("".join(instances))
        cfile.write(C_FILE_FOOTER)
        cfile.write("\n".join(init_calls))
        cfile.write(C_FILE_INIT_CALLS)

    # Writing the header file
    with open(h_file_path, "w") as hfile:
        hfile.write(HEADER_FILE_HEADER_TEMPLATE.format(date=current_date))
        hfile.write("".join(headers))
        hfile.write(HEADER_FILE_FOOTER)

# Argument parser configuration
parser = argparse.ArgumentParser(description='Generates platform_conf.c and platform_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, required=True, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, required=True, help='Destination folder for generated files.')

# Parsing arguments
args = parser.parse_args()

# Executing the script with provided arguments
generate_platform_files(args.input, args.output)
