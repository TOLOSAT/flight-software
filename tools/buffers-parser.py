import csv
from datetime import datetime
import argparse
import os

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description="Génère les fichiers buffers_conf.c et buffers_conf.h à partir d'un fichier CSV.")
parser.add_argument('-i', '--input', type=str, help='Chemin du fichier CSV d\'entrée.')
parser.add_argument('-o', '--output', type=str, help='Dossier de destination pour les fichiers générés.')

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
                "Sender Ref": row[" \"Sender Ref\""].strip(),
                "Receiver Ref": row[" \"Receiver Ref\""].strip(),
                "Msg Size": row[" \"Msg Size\""].strip(),
                "Msg Nb": row[" \"Msg Nb\""].strip()
            })

    header_c = f"""/**
 * @file    buffers_conf.c
 * @author  Merlin Kooshmanian
 * @brief   Source file stocking configuration table for buffers
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/buffers_conf.h"
#include "conf/tasks_conf.h"

/*************************** Variables Definitions ***************************/

"""

    header_h = f"""/**
 * @file    buffers_conf.h
 * @author  Merlin Kooshmanian
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
 * @var     g_buffers_static_conf
 * @brief   Configuration table where all buffers static parameters are stored
 */
"""
    buffer_dynamic_conf_comment = """/**
 * @var     g_buffers_dynamic_conf
 * @brief   Configuration table where all buffers dynamic parameters are stored
 */
"""
    buffer_data_definitions = ""
    buffer_static_conf = buffer_static_conf_comment + "const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS] = \n{\n"
    buffer_dynamic_conf = buffer_dynamic_conf_comment + "bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS] = \n{\n"
    buffer_data_declarations = """/*************************** Variables Declarations **************************/
    
extern const bufferStaticConf_t g_buffers_static_conf[NB_BUFFERS];
extern bufferDynamicConf_t g_buffers_dynamic_conf[NB_BUFFERS];
"""
    
    for i, buffer in enumerate(buffers):
        buffer_ref = buffer["Buffer Ref"]
        buffer_defs += f'#define {buffer_ref}_MSG_SIZE {buffer["Msg Size"]} /**< {buffer_ref} Message Size */\n'
        buffer_defs += f'#define {buffer_ref}_MSG_NB {buffer["Msg Nb"]} /**< {buffer_ref} Message Number */\n'
        buffer_enum += f"    {buffer_ref},\n"
        buffer_static_conf += f"    {{ {buffer_ref}, {buffer['Sender Ref']}, {buffer['Receiver Ref']}, {buffer_ref}_MSG_SIZE, {buffer_ref}_MSG_NB }},\n"
        buffer_dynamic_conf += f"    {{.buffer_data = g_{buffer_ref.lower()}_data}},\n"
        buffer_data_declarations += f"extern bufferData_t g_{buffer_ref.lower()}_data[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB];\n"
        buffer_data_definitions += f"""
/**
 * @var     g_{buffer_ref.lower()}_data
 * @brief   Data array for {buffer_ref}
 */
bufferData_t g_{buffer_ref.lower()}_data[{buffer_ref}_MSG_SIZE*{buffer_ref}_MSG_NB] = {{0}};
"""

    buffer_enum += "    NB_BUFFERS\n};\n\n"
    buffer_static_conf += "};\n\n"
    buffer_dynamic_conf += "};\n"

    with open(h_file_name, 'w') as h_file:
        h_file.write(header_h + buffer_defs + buffer_enum + buffer_data_declarations)
        h_file.write("\n#endif /* BUFFERS_CONF_H */\n")

    with open(c_file_name, 'w') as c_file:
        c_file.write(header_c + buffer_static_conf + buffer_dynamic_conf + buffer_data_definitions)

    print(f"Les fichiers '{c_file_name}' et '{h_file_name}' ont été générés avec succès.")

if __name__ == "__main__":
    generate_buffers_conf(csv_file_name, output_directory)
