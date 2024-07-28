import csv
import argparse
import os
from datetime import datetime

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Generates mutex_conf.c and mutex_conf.h files from a CSV file.')
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

c_file_name = os.path.join(output_directory, 'mutex_conf.c')
h_file_name = os.path.join(output_directory, 'mutex_conf.h')

# Obtention de la date actuelle pour l'en-tête
current_date = datetime.now().strftime("%d/%m/%Y")

mutex_refs = []

try:
    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            mutex_refs.append(row["Mutex Ref"])

    with open(c_file_name, 'w') as c_file:
        c_file.write(f"""/**
 * @file    mutex_conf.c
 * @brief   Source file stocking configuration table for mutex
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/mutex_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CONF_TABLES_SECTION  __attribute__((section(".conf_tables")))    /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION  __attribute__((section(".desc_tables")))    /**< Descriptor table goes to .desc_tables section */
#define IN_MUTEX_DATA_SECTION   __attribute__((section(".mutex_data")))     /**< Mutex data go to .mutex_data section */

/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutex_conf_table
 * @brief   Configuration table where all mutexes configuration are stored
 */
const mutexConf_t IN_CONF_TABLES_SECTION g_mutex_conf_table[NB_MUTEXES] = 
{{
""")
        for ref in mutex_refs:
            c_file.write(f"    {{.p_data = &g_{ref.lower()}_data}}, /* {ref} */\n")
        c_file.write("};\n\n")

        c_file.write(f"""/**
 * @var     g_mutex_desc_table
 * @brief   Configuration table where all mutexes descriptors are stored
 */
mutexDesc_t IN_DESC_TABLES_SECTION g_mutex_desc_table[NB_MUTEXES] = {{0}};
""")

        for ref in mutex_refs:
            c_file.write(f"""
/**
 * @var     g_{ref.lower()}_data
 * @brief   Data array for {ref}
 */
mutexData_t IN_MUTEX_DATA_SECTION g_{ref.lower()}_data = {{0}};
""")

    with open(h_file_name, 'w') as h_file:
        h_file.write(f"""/**
 * @file    mutex_conf.h
 * @brief   Header file stocking configuration table for mutex
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef MUTEX_CONF_H
#define MUTEX_CONF_H

/******************************* Include Files *******************************/

#include "mutex.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    MUTEX_ENUM
 * @brief   Enum defining mutexes reference numbers
 */
enum MUTEX_ENUM 
{{
""")
        for ref in mutex_refs:
            h_file.write(f"    {ref},\n")
        h_file.write("""    NB_MUTEXES,
};

/*************************** Variables Declarations **************************/

extern const mutexConf_t g_mutex_conf_table[NB_MUTEXES];
extern mutexDesc_t g_mutex_desc_table[NB_MUTEXES];
""")
        for ref in mutex_refs:
            h_file.write(f"extern mutexData_t g_{ref.lower()}_data;\n")
        h_file.write("""
#endif /* MUTEX_CONF_H */\n""")

    print(f"Files '{c_file_name}' and '{h_file_name}' have been generated with success.")
except Exception as e:
    print(f"Error when generating : {e}")
