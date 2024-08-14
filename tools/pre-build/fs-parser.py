import csv
import argparse
import os
from datetime import datetime

# Argument parser configuration
parser = argparse.ArgumentParser(description='Generates fs_conf.c and fs_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, help='Destination folder for generated files.')

# Parse the arguments
args = parser.parse_args()

if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Check if the output directory exists, if not, create it
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, 'fs_conf.c')
h_file_name = os.path.join(output_directory, 'fs_conf.h')

# Get the current date for the header
current_date = datetime.now().strftime("%d/%m/%Y")

try:
    file_refs, file_names, file_access_modes, auto_sync_modes = [], [], [], []

    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            file_refs.append(row["File Ref"])
            file_names.append(row["File Name"].replace('"', '').strip())
            file_access_modes.append(row["File Access Mode"].strip())
            auto_sync_modes.append(row["Auto Synchronisation"].strip())

    with open(c_file_name, 'w') as c_file:
        c_file.write(f"""/**
 * @file    fs_conf.c
 * @brief   Source file storing configuration for file system content
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/fs_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CONF_TABLES_SECTION  __attribute__((section(".conf_tables")))    /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION  __attribute__((section(".desc_tables")))    /**< Descriptor table goes to .desc_tables section */
#define IN_TMPFS_SECTION        __attribute__((section(".tmpfs")))          /**< Temporary file goes to .tmpfs section */

/*************************** Variables Definitions ***************************/

/**
 * @var     g_file_desc_table
 * @brief   Configuration table where all file descriptors are stored
 */
fsFileDesc_t IN_DESC_TABLES_SECTION g_file_desc_table[NB_FILES] = 
{{
    /* Fileno , File Name , File Access Mode , Temp File , Auto Sync */
""")
        for ref, name, mode, auto_sync in zip(file_refs, file_names, file_access_modes, auto_sync_modes):
            temp_file_var = f"{ref.lower()}_temp_file"
            c_file.write(f"    {{ {ref} , \"{name}\" , {mode} , {auto_sync} , &{temp_file_var} }},\n")
        c_file.write("};\n")
        for ref in file_refs:
            temp_file_var = f"{ref.lower()}_temp_file"
            c_file.write(f"""
/**
 * @var     {temp_file_var}
 * @brief   Temporary file used for {ref}
 */
FIL IN_TMPFS_SECTION {temp_file_var} = {{0}};
""")

    with open(h_file_name, 'w') as h_file:
        h_file.write(f"""/**
 * @file    fs_conf.h
 * @brief   Header file storing configuration for file system content
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef FS_CONF_H
#define FS_CONF_H

/******************************* Include Files *******************************/

#include "fs.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    FILE_ENUM
 * @brief   Enum defining file reference numbers
 */
enum FILE_ENUM 
{{\n""")
        for ref in file_refs:
            h_file.write(f"    {ref},\n")
        h_file.write("""    NB_FILES,
};

/*************************** Variables Declarations **************************/

extern fsFileDesc_t g_file_desc_table[NB_FILES];""")
        for ref in file_refs:
            temp_file_var = f"{ref.lower()}_temp_file"
            h_file.write(f"\nextern FIL {temp_file_var};")
        h_file.write("\n\n#endif /* FS_CONF_H */\n")

    print(f"Files '{c_file_name}' and '{h_file_name}' have been generated successfully.")
except Exception as e:
    print(f"Error when generating: {e}")
