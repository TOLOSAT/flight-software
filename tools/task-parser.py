import csv
from datetime import datetime

# Nom du fichier CSV à lire
csv_file_name = 'input.csv'
# Nom des fichiers C et H à générer
c_file_name = 'tasks_conf.c'
h_file_name = 'tasks_conf.h'

# Obtention de la date actuelle pour l'en-tête
current_date = datetime.now().strftime("%d/%m/%Y")

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

/***************************** Types Definitions *****************************/

/**
 * @enum    TASKS_ENUM
 * @brief   Enum defining tasks reference numbers
 */
enum TASKS_ENUM {{
"""

footer_h = """    NB_TASKS,
};

/*************************** Variables Declarations **************************/

extern const taskStaticConf_t g_tasks_static_conf[NB_TASKS];
extern taskDynamicConf_t g_tasks_dynamic_conf[NB_TASKS];
"""

footer_c = """

/*************************** Variables Definitions ***************************/

/**
 * @var     g_tasks_static_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskStaticConf_t g_tasks_static_conf[NB_TASKS] = 
{
    /* Task Ref , Name , Function , Priority , Stack Size , Default Period , Default Deadline, Privilege , Memory Regions */
"""

# Fonction pour convertir une ligne CSV en ligne C avec les déclarations appropriées
def csv_to_c_row(row):
    task_ref = row[0]  # Utilisation du champ "Task Ref" pour l'enum
    name = row[1].replace('"', '')  # Nettoyage des guillemets autour du nom
    memory_regions = ', '.join(['{{' + x + '}}' for x in row[8:]]) + ' '
    c_row = f'    {{ {task_ref.upper().replace(" ", "_")} , "{name}" , {row[2]} , {row[3]} , {row[4]} , {row[5]} , {row[6]} , {row[7]} , {memory_regions}}},\n'
    return c_row

# Fonction pour générer les définitions de stack
def generate_stack_definitions(task_refs, task_names, stack_sizes):
    stack_definitions = ""
    for ref, name, size in zip(task_refs, task_names, stack_sizes):
        formatted_name = name.lower().replace('task', '_task')  # Formatage du nom pour respecter le style
        stack_definitions += f"""
/**
 * @var     g_{formatted_name}_stack
 * @brief   Stack for {name}
 */
taskStack_t g_{formatted_name}_stack[{size}] = {{0}};
"""
    return stack_definitions

try:
    task_refs = []  # Liste pour stocker les références de tâches
    task_names = []  # Liste pour stocker les noms des tâches
    stack_sizes = []  # Liste pour stocker les tailles des stacks

    with open(csv_file_name, mode='r', newline='') as csv_file:
        csv_reader = csv.reader(csv_file)
        next(csv_reader)  # On saute la première ligne (entêtes)
        
        with open(c_file_name, 'w') as c_file:
            c_file.write(header_c)
            
            c_file.write("""/**
 * @var     g_tasks_static_conf
 * @brief   Configuration table where all tasks static parameters are stored
 */
const taskStaticConf_t g_tasks_static_conf[NB_TASKS] = 
{
    /* Task Ref , Name , Function , Priority , Stack Size , Default Period , Default Deadline, Privilege , Memory Regions */
""")
            
            for row in csv_reader:
                c_row = csv_to_c_row(row)
                c_file.write(c_row)
                
                # Collecter les références de tâches, les noms des tâches et les tailles des stacks
                task_ref = row[0]
                task_name = row[1].replace('"', '')
                stack_size = row[4]
                task_refs.append(task_ref)
                task_names.append(task_name)
                stack_sizes.append(stack_size)
            
            c_file.write("};\n")
            stack_definitions = generate_stack_definitions(task_refs, task_names, stack_sizes)
            c_file.write(stack_definitions)

    with open(h_file_name, 'w') as h_file:
        h_file.write(header_h)
        for ref in task_refs:
            h_file.write(f'    {ref.upper().replace(" ", "_")},\n')
        h_file.write(footer_h)
        for name, size in zip(task_names, stack_sizes):
            formatted_name = name.lower().replace('task', '_task')
            h_file.write(f"extern taskStack_t g_{formatted_name}_stack[{size}];\n")
        h_file.write("\n#endif /* TASKS_CONF_H */\n")

    print(f"Les fichiers '{c_file_name}' et '{h_file_name}' ont été générés avec succès.")
except Exception as e:
    print(f"Erreur lors de la conversion : {e}")
