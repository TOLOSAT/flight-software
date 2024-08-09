import csv
import argparse
import os
from datetime import datetime

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Generates hk_conf.c and hk_conf.h files from a CSV file.')
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

c_file_name = os.path.join(output_directory, 'hk_conf.c')
h_file_name = os.path.join(output_directory, 'hk_conf.h')

# Obtention de la date actuelle pour l'en-tête
current_date = datetime.now().strftime("%Y-%m-%d")

try:
    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        hk_refs = [(row["HK Ref"], row["HKID"], row["HK Status"]) for row in csv_reader]

    with open(c_file_name, 'w') as c_file:
        c_file.write(f"""/**
 * @file    hk_conf.c
 * @brief   Source file stocking configuration table for housekeeping parameters
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/hk_conf.h"

/***************************** Macros Definitions ****************************/

#define IN_CONF_TABLES_SECTION   __attribute__((section(".conf_tables")))     /**< Conf table goes to .conf_tables section */
#define IN_DESC_TABLES_SECTION   __attribute__((section(".desc_tables")))     /**< Descriptor table goes to .desc_tables section */

/*************************** Variables Definitions ***************************/

/**
 * @var     g_hk_desc_table
 * @brief   Configuration table where all housekeeping parameters are stored
 */
pusHkConf_t IN_DESC_TABLES_SECTION g_hk_desc_table[NB_HK] = 
{{
""")
        for ref, hkid, status in hk_refs:
            c_file.write(f'    /* HK Ref, HKID, HK Status */\n    {{ {ref}, {hkid}, {status} }},\n')
        c_file.write("};\n")

    with open(h_file_name, 'w') as h_file:
        h_file.write(f"""/**
 * @file    hk_conf.h
 * @brief   Header file stocking configuration table for housekeeping parameters
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef HK_CONF_H
#define HK_CONF_H

/******************************* Include Files *******************************/

#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

/***************************** Types Definitions *****************************/

/**
 * @enum    HK_ENUM
 * @brief   Enum defining housekeeping reference numbers
 */
enum HK_ENUM 
{{
""")
        for ref, _, _ in hk_refs:
            h_file.write(f'    {ref},\n')
        h_file.write("""    NB_HK,
};

/*************************** Variables Declarations **************************/

extern pusHkConf_t g_hk_desc_table[NB_HK];

#endif /* HK_CONF_H */\n""")

    print(f"Files '{c_file_name}' and '{h_file_name}' have been generated with success.")
except Exception as e:
    print(f"Error when generating : {e}")
