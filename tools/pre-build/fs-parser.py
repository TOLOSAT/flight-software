import csv
import argparse
import os
from datetime import datetime

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Generates fs_conf.c and fs_conf.h files from a CSV file.')
parser.add_argument('-i', '--input', type=str, help='Path to input CSV file.')
parser.add_argument('-o', '--output', type=str, help='Destination folder for generated files.')

# Analyse des arguments
args = parser.parse_args()

if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Vérifie si le dossier de sortie existe, sinon le crée
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, 'fs_conf.c')
h_file_name = os.path.join(output_directory, 'fs_conf.h')

# Obtention de la date actuelle pour l'en-tête
current_date = datetime.now().strftime("%d/%m/%Y")

try:
    file_refs, file_names, file_access_modes = [], [], []

    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            file_refs.append(row["File Ref"])
            file_names.append(row["File Name"].replace('"', '').strip())
            file_access_modes.append(row["File Access Mode"].strip())

    with open(c_file_name, 'w') as c_file:
        c_file.write(f"""/**
 * @file    fs_conf.c
 * @brief   Source file stocking configuration for file system content
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
 * @brief   Configuration table where all files descriptors are stored
 */
const fsFileDesc_t IN_DESC_TABLES_SECTION g_file_desc_table[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES] = 
{{
    /* Fileno , File Name , File Access Mode , Temp File */
    {{\n""")
        for ref, name, mode in zip(file_refs, file_names, file_access_modes):
            temp_file_var = f"{ref.lower()}_temp_file"
            c_file.write(f"    {{ {ref} , \"{name}\" , {mode} , &{temp_file_var} }},\n")
        c_file.write("""    }
};
""")
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
 * @brief   Header file stocking configuration for file system content
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef FS_CONF_H
#define FS_CONF_H

/******************************* Include Files *******************************/

#include "fs_types.h"

/***************************** Macros Definitions ****************************/

#define MAX_NB_FILES_PER_DEVICES    (NB_SD0_FILES)  /**< Maximum Number of files */

/***************************** Types Definitions *****************************/

/**
 * @enum    MEMORY_DEVICES_ENUM
 * @brief   Enum defining memory devices reference numbers
 */
enum MEMORY_DEVICES_ENUM 
{{
    SD0 = 0u,
    NB_MEMORY_DEVICES,
}};

/**
 * @enum    FILE_SD0_ENUM
 * @brief   Enum defining file in SD0 reference numbers
 */
enum FILE_SD0_ENUM 
{{\n""")
        for ref in file_refs:
            h_file.write(f"    {ref},\n")
        h_file.write("""    NB_SD0_FILES,
};

/*************************** Variables Declarations **************************/

extern const fsFileDesc_t g_file_desc_table[NB_MEMORY_DEVICES][MAX_NB_FILES_PER_DEVICES];""")
        for ref in file_refs:
            temp_file_var = f"{ref.lower()}_temp_file"
            h_file.write(f"\nextern FIL {temp_file_var};")
        h_file.write("\n\n#endif /* FS_CONF_H */\n")

    print(f"Files '{c_file_name}' and '{h_file_name}' have been generated with success.")
except Exception as e:
    print(f"Error when generating : {e}")
