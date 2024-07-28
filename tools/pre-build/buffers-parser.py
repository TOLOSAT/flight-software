import csv
from datetime import datetime
import argparse
import os

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description="Generates buffers_conf.c and buffers_conf.h files from a CSV file.")
parser.add_argument('-i', '--input', type=str, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, help='Destination folder for generated files.')

args = parser.parse_args()

# Vérification de la présence des arguments nécessaires
if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Vérifie si le dossier de sortie existe, sinon le crée
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, 'buffers_conf.c')
h_file_name = os.path.join(output_directory, 'buffers_conf.h')

# Obtention de la date actuelle pour l'en-tête
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
 * @brief   Source file stocking configuration table for buffers
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CONF_TABLE_SECTION    __attribute__((section(".conf_tables")))      /**< Conf table goes to .conf_tables section */
#define IN_DESCRIPTOR_TABLES_SECTION   __attribute__((section(".desc_tables")))     /**< Descriptor table goes to .desc_tables section */
#define IN_BUFFER_DATA_SECTION          __attribute__((section(".buffer_data")))            /**< Buffer data go to .buffer_data section */

/*************************** Variables Definitions ***************************/

"""

    header_h = f"""/**
 * @file    buffers_conf.h
 * @brief   Header file for buffer configuration
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef BUFFERS_CONF_H
#define BUFFERS_CONF_H

/******************************* Include Files *******************************/

#include "buffers.h"
#include "pus_constants.h"
#include "life.h"
#include "events.h"
#include "housekeeping.h"

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
 * @brief   Configuration table where all buffers static parameters are stored
 */
"""
    buffer_dynamic_conf_comment = """/**
 * @var     g_buffer_desc_table
 * @brief   Configuration table where all buffers descriptors are stored
 */
"""
    buffer_data_definitions = ""
    buffer_static_conf = buffer_static_conf_comment + "const bufferConf_t IN_CONF_TABLE_SECTION g_buffers_conf[NB_BUFFERS] = \n{\n"
    buffer_dynamic_conf = buffer_dynamic_conf_comment + "bufferDesc_t IN_DESCRIPTOR_TABLES_SECTION g_buffer_desc_table[NB_BUFFERS] = \n{\n"
    buffer_data_declarations = """/*************************** Variables Declarations **************************/
    
extern const bufferConf_t g_buffers_conf[NB_BUFFERS];
extern bufferDesc_t g_buffer_desc_table[NB_BUFFERS];
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
        buffer_static_conf += f"    {{ {buffer_ref}, {buffer['Sender Ref']}, {buffer['Receiver Ref']}, {buffer_ref}_MSG_SIZE, {buffer_ref}_MSG_NB }},\n"
        buffer_dynamic_conf += f"    {{.buffer_data = g_{buffer_ref.lower()}_data}},\n"
        buffer_data_declarations += f"extern bufferData_t g_{buffer_ref.lower()}_data[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB];\n"
        buffer_data_definitions += f"""
/**
 * @var     g_{buffer_ref.lower()}_data
 * @brief   Data array for {buffer_ref}
 */
bufferData_t IN_BUFFER_DATA_SECTION g_{buffer_ref.lower()}_data[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB] = {{0}};
"""

    buffer_enum += "    NB_BUFFERS\n};\n\n"
    buffer_static_conf += "};\n\n"
    buffer_dynamic_conf += "};\n"

    with open(h_file_name, 'w') as h_file:
        h_file.write(header_h + buffer_defs + buffer_enum + buffer_data_declarations)
        h_file.write("\n#endif /* BUFFERS_CONF_H */\n")

    with open(c_file_name, 'w') as c_file:
        c_file.write(header_c + buffer_static_conf + buffer_dynamic_conf + buffer_data_definitions)

    print(f"Files '{c_file_name}' and '{h_file_name}' have been generated with success.")

if __name__ == "__main__":
    generate_buffers_conf(csv_file_name, output_directory)
