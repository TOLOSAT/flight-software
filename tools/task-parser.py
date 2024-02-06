import csv
from datetime import datetime
import argparse
import os

# Configuration de l'analyseur d'arguments
parser = argparse.ArgumentParser(description='Génère les fichiers tasks_conf.c et tasks_conf.h à partir d\'un fichier CSV.')
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

c_file_name = os.path.join(output_directory, 'tasks_conf.c')
h_file_name = os.path.join(output_directory, 'tasks_conf.h')

# Obtention de la date actuelle pour l'en-tête
current_date = datetime.now().strftime("%d/%m/%Y")

# En-tête pour le fichier .c
header_c = f"""/**
 * @file    tasks_conf.c
 * @brief   Source file stocking configuration table for tasks
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "conf/tasks_conf.h"
#include "dummy_tasks.h"

/*************************** Variables Definitions ***************************/
"""

# En-tête pour le fichier .h
header_h = f"""/**
 * @file    tasks_conf.h
 * @brief   Header file stocking configuration table for tasks
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TASKS_CONF_H
#define TASKS_CONF_H

/******************************* Include Files *******************************/

#include "tasks.h"

/***************************** Macros Definitions ****************************/

"""

# Fonction pour générer les macros de taille de stack dans le .h avec commentaires
def generate_stack_macros(task_refs, stack_sizes):
    macros = "\n"
    for ref, size in zip(task_refs, stack_sizes):
        task_ref_macro = f"{ref.upper().replace(' ', '_')}_STACK_SIZE"
        macros += f"#define {task_ref_macro} {size} /** {ref} Stack Size*/\n"
    return macros + "\n"

# Fonction pour convertir une ligne CSV en ligne C pour la table static
def csv_to_c_static_row(row):
    task_ref = row["Task Ref"].upper().replace(' ', '_')
    name = row["Name"].replace('"', '')
    function = row["Function"]
    priority = row["Priority"]
    stack_size_macro = f"{task_ref}_STACK_SIZE"
    default_period = row["Default Period"]
    default_deadline = row["Default Deadline"]
    privilege = row["Privilege"]
    memory_regions = '{' + ', '.join([x for x in row.values()][8:]) + '}'

    return f'    {{ {task_ref}, "{name}", {function}, {priority}, {stack_size_macro}, {default_period}, {default_deadline}, {privilege}, {memory_regions} }},\n'

# Fonction pour générer la table dynamic et les définitions de stack
def generate_dynamic_and_stack_definitions(task_refs, task_names):
    dynamic_conf = """
/**
 * @var     g_tasks_dynamic_conf
 * @brief   Configuration table where all tasks dynamic parameters are stored
 */
taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS] = 
{
"""
    stack_definitions = ""
    for ref, name in zip(task_refs, task_names):
        formatted_ref = ref.upper().replace(' ', '_')
        stack_name = f"g_{formatted_ref.lower()}_stack"
        dynamic_conf += f"    {{ 0u, TASK_NOMINAL, 0u, 0u, 0u, {{0u}}, {stack_name} }}, /* {formatted_ref} */\n"
        stack_definitions += f"""
/**
 * @var     {stack_name}
 * @brief   Stack for {name}
 */
taskStack_t {stack_name}[{formatted_ref}_STACK_SIZE] = {{0}};
"""
    dynamic_conf += "};\n"
    return dynamic_conf, stack_definitions

try:
    task_refs = []  # Pour stocker les références des tâches
    task_names = []  # Pour stocker les noms des tâches
    stack_sizes = []  # Pour stocker les tailles des stacks

    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        
        tasks_data = [row for row in csv_reader]
        for row in tasks_data:
            task_refs.append(row["Task Ref"])
            task_names.append(row["Name"].replace('"', ''))
            stack_sizes.append(row["Stack Size"])

        stack_macros = generate_stack_macros(task_refs, stack_sizes)
        
        with open(h_file_name, 'w') as h_file, open(c_file_name, 'w') as c_file:
            h_file.write(header_h + stack_macros)
            h_file.write("/***************************** Types Definitions *****************************/\n\n")
            h_file.write("enum TASKS_ENUM {\n")
            for ref in task_refs:
                h_file.write(f"    {ref.upper().replace(' ', '_')},\n")
            h_file.write("    NB_TASKS\n};\n\n")
            h_file.write("extern const taskStaticConf_t g_tasks_static_conf[NB_TASKS];\n")
            h_file.write("extern taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS];\n")
            for ref in task_refs:
                h_file.write(f"extern taskStack_t g_{ref.lower().replace(' ', '_')}_stack[{ref.upper().replace(' ', '_')}_STACK_SIZE];\n")
            h_file.write("\n#endif /* TASKS_CONF_H */\n")

            c_file.write(header_c)
            c_file.write("""
/**
 * @var     g_tasks_static_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskStaticConf_t g_tasks_static_conf[NB_TASKS] = 
{
""")
            for row in tasks_data:
                c_file.write(csv_to_c_static_row(row))
            c_file.write("};\n")

            dynamic_conf, stack_definitions = generate_dynamic_and_stack_definitions(task_refs, task_names)
            c_file.write(dynamic_conf)
            c_file.write(stack_definitions)

    print(f"Les fichiers '{c_file_name}' et '{h_file_name}' ont été générés avec succès.")
except Exception as e:
    print(f"Erreur lors de la conversion : {e}")
