import csv
import argparse
import os
from datetime import datetime

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Génère les fichiers hk_conf.c et hk_conf.h à partir d\'un fichier CSV.')
parser.add_argument('-i', '--input', type=str, help='Chemin du fichier CSV d\'entrée.')
parser.add_argument('-o', '--output', type=str, help='Dossier de destination pour les fichiers générés.')

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
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/hk_conf.h"

/*************************** Variables Definitions ***************************/

/**
 * @var     g_hk_conf
 * @brief   Configuration table where all housekeeping parameters are stored
 */
pusHkConf_t IN_DYNAMIC_CONF_TABLE_SECTION g_hk_conf[NB_HK] = 
{{
""")
        for ref, hkid, status in hk_refs:
            c_file.write(f'    /* HK Ref, HKID, HK Status */\n    {{ {ref}, {hkid}, {status} }},\n')
        c_file.write("};\n")

    with open(h_file_name, 'w') as h_file:
        h_file.write(f"""/**
 * @file    hk_conf.h
 * @brief   Header file stocking configuration table for housekeeping parameters
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef HK_CONF_H
#define HK_CONF_H

/******************************* Include Files *******************************/

#include "services/pus3.h"

/***************************** Macros Definitions ****************************/

#define IN_STATIC_CONF_TABLE_SECTION    __attribute__((section(".static_conf_table")))      /**< Static conf table goes to .static_conf_table section */
#define IN_DYNAMIC_CONF_TABLE_SECTION   __attribute__((section(".dynamic_conf_table")))     /**< Dynamic conf table goes to .dynamic_conf_table section */

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

extern pusHkConf_t g_hk_conf[NB_HK];

#endif /* HK_CONF_H */\n""")

    print(f"Les fichiers '{c_file_name}' et '{h_file_name}' ont été générés avec succès.")
except Exception as e:
    print(f"Erreur lors de la génération des fichiers : {e}")
