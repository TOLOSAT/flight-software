import csv
import argparse
import os
from datetime import datetime

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Génère les fichiers mutex_conf.c et mutex_conf.h à partir d\'un fichier CSV.')
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

/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutex_conf
 * @brief   Configuration table where all mutexes parameters are stored
 */
mutexConf_t IN_DYNAMIC_CONF_TABLE_SECTION g_mutex_conf[NB_MUTEXES] = 
{{
    /* Mutex Ref , Mutex Handler */
""")
        for ref in mutex_refs:
            c_file.write(f"    {{ {ref} , 0u }},\n")
        c_file.write("};\n")

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

#define IN_STATIC_CONF_TABLE_SECTION    __attribute__((section(".static_conf_table")))      /**< Static conf table goes to .static_conf_table section */
#define IN_DYNAMIC_CONF_TABLE_SECTION   __attribute__((section(".dynamic_conf_table")))     /**< Dynamic conf table goes to .dynamic_conf_table section */

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

extern mutexConf_t g_mutex_conf[NB_MUTEXES];

#endif /* MUTEX_CONF_H */\n""")

    print(f"Les fichiers '{c_file_name}' et '{h_file_name}' ont été générés avec succès.")
except Exception as e:
    print(f"Erreur lors de la génération des fichiers : {e}")
