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
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/

""")

        # Ajouter toutes les déclarations de mutex queues ici
        for ref in mutex_refs:
            c_file.write(f"static mutexQueue_t {ref.lower()}_queue;\n")

        c_file.write("""
/*************************** Variables Definitions ***************************/

/**
 * @var     g_mutex_conf_table
 * @brief   Configuration table where all mutexes configuration are stored
 */
const mutexConf_t IN_CONF_TABLES_SECTION g_mutex_conf_table[NB_MUTEXES] = 
{
""")
        for ref in mutex_refs:
            c_file.write(f"    {{.p_queue = &{ref.lower()}_queue}}, /* {ref} */\n")
        c_file.write("};\n\n")

        c_file.write(f"""/**
 * @var     g_mutexes_desc_table
 * @brief   Configuration table where all mutexes descriptors are stored
 */
mutexDesc_t IN_DESC_TABLES_SECTION g_mutexes_desc_table[NB_MUTEXES] = {{0}};
""")

        # Définir chaque mutex queue après les déclarations
        for ref in mutex_refs:
            c_file.write(f"""
/**
 * @var     {ref.lower()}_queue
 * @brief   Queue array for {ref}
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {ref.lower()}_queue = {{0}};
""")

    with open(h_file_name, 'w') as h_file:
        h_file.write(f"""/**
 * @file    mutex_conf.h
 * @brief   Header file stocking configuration table for mutex
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef MUTEX_CONF_H
#define MUTEX_CONF_H

/******************************* Include Files *******************************/

#include "sync/mutex.h"

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
extern mutexDesc_t g_mutexes_desc_table[NB_MUTEXES];

#endif /* MUTEX_CONF_H */
""")

except Exception as e:
    print(f"Error when generating : {e}")
