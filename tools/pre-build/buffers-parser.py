import csv
from datetime import datetime
import argparse
import os

# Argument parser configuration
parser = argparse.ArgumentParser(description="Generates buffers_conf.c and buffers_conf.h files from a CSV file.")
parser.add_argument('-i', '--input', type=str, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, help='Destination folder for generated files.')

args = parser.parse_args()

# Check for required arguments
if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Create output directory if it doesn't exist
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, 'buffers_conf.c')
h_file_name = os.path.join(output_directory, 'buffers_conf.h')

# Get current date for the header
current_date = datetime.now().strftime("%d/%m/%Y")

def generate_buffers_conf(csv_file_name, output_directory):
    buffers = []

    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            buffers.append({
                "Buffer Ref": row["Buffer Ref"].strip(),
                "Sender Ref": row["Sender Ref"].strip(),
                "Receiver Ref": row["Receiver Ref"].strip(),
                "Msg Size": row["Msg Size"].strip(),
                "Msg Nb": row["Msg Nb"].strip()
            })

    header_c = f"""/**
 * @file    buffers_conf.c
 * @brief   Source file storing configuration table for buffers
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

/***************************** Macros Definitions ****************************/

"""

    header_h = f"""/**
 * @file    buffers_conf.h
 * @brief   Header file for buffer configuration
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/******************************* Include Files *******************************/

#include "base/buffers.h"

/***************************** Macros Definitions ****************************/
"""

    buffer_defs = ""
    buffer_enum = "\n/***************************** Types Definitions *****************************/\n\n"
    buffer_enum += """/**
 * @enum    BUFFERS_ENUM
 * @brief   Enum defining buffers reference numbers
 */
enum BUFFERS_ENUM {
"""
    buffer_static_conf_comment = """/**
 * @var     g_buffers_conf
 * @brief   Configuration table where all buffers' static parameters are stored
 */
"""
    buffer_static_conf = buffer_static_conf_comment + "const bufferConf_t IN_CONF_TABLES_SECTION g_buffers_conf[NB_BUFFERS] = \n{\n"
    buffer_dynamic_conf_comment = """/**
 * @var     g_buffers_desc_table
 * @brief   Configuration table where all buffers' descriptors are stored
 */
"""
    buffer_dynamic_conf = buffer_dynamic_conf_comment + "bufferDesc_t IN_DESC_TABLES_SECTION g_buffers_desc_table[NB_BUFFERS] = {0};\n"
    buffer_array_declarations = """/*************************** Variables Declarations **************************/
    
extern const bufferConf_t g_buffers_conf[NB_BUFFERS];
extern bufferDesc_t g_buffers_desc_table[NB_BUFFERS];
"""

    for i, buffer in enumerate(buffers):
        buffer_ref = buffer["Buffer Ref"]
        buffer_size = buffer["Msg Size"]
        if buffer_size.isdigit():
            buffer_size += "u"
        buffer_depth = buffer["Msg Nb"]
        if buffer_depth.isdigit():
            buffer_depth += "u"
        buffer_defs += f'#define {buffer_ref}_MSG_SIZE {buffer_size} /**< {buffer_ref} Message Size */\n'
        buffer_defs += f'#define {buffer_ref}_MSG_NB {buffer_depth} /**< {buffer_ref} Message Number */\n'
        buffer_enum += f"    {buffer_ref},\n"
        buffer_static_conf += f"    {{ {buffer_ref}, {buffer['Sender Ref']}, {buffer['Receiver Ref']}, {buffer_ref}_MSG_SIZE, {buffer_ref}_MSG_NB, &{buffer_ref.lower()}_entity, {buffer_ref.lower()}_array }},\n"

    buffer_enum += "    NB_BUFFERS\n};\n\n"
    buffer_static_conf += "};\n\n"

    buffer_array_definitions = ""
    buffer_entity_definitions = ""

    for buffer in buffers:
        buffer_ref = buffer["Buffer Ref"]
        buffer_array_definitions += f"""
/**
 * @var     {buffer_ref.lower()}_array
 * @brief   Data array for {buffer_ref}
 */
static bufferArray_t IN_BUFFER_ARRAYS_SECTION {buffer_ref.lower()}_array[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB] = {{0}};
"""
        buffer_entity_definitions += f"""
/**
 * @var     {buffer_ref.lower()}_entity
 * @brief   Entity structure for {buffer_ref}
 */
static bufferEntity_t IN_BUFFER_ENTITIES_SECTION {buffer_ref.lower()}_entity = {{0}};
"""

    with open(h_file_name, 'w') as h_file:
        # Write the header file content without extern declarations for arrays and entities
        h_file.write(header_h + buffer_enum + buffer_array_declarations)
        h_file.write("\n#endif /* BUFFERS_CONF_H */\n")

    with open(c_file_name, 'w') as c_file:
        c_file.write(header_c + buffer_defs)  # Write the macros to the .c file
        c_file.write("\n/*************************** Variables Declarations **************************/\n\n")
        
        # Static buffer array and entity declarations
        for buffer in buffers:
            buffer_ref = buffer["Buffer Ref"]
            c_file.write(f"static bufferArray_t {buffer_ref.lower()}_array[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB];\n")
        c_file.write(f"\n")
        for buffer in buffers:
            buffer_ref = buffer["Buffer Ref"]
            c_file.write(f"static bufferEntity_t {buffer_ref.lower()}_entity;\n")

        c_file.write("\n/*************************** Variables Definitions ***************************/\n\n")
        c_file.write(buffer_static_conf + buffer_dynamic_conf + buffer_entity_definitions + buffer_array_definitions)

if __name__ == "__main__":
    generate_buffers_conf(csv_file_name, output_directory)
