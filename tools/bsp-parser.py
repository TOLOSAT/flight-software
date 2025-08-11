#!/usr/bin/env python3

import os
import json
import argparse
from datetime import datetime

# ==============================================================================
# ==================== Generation of peripherals configuration =================
# ==============================================================================

def generate_peripherals_conf(peripherals, output_directory):
    current_date = datetime.now().strftime("%d/%m/%Y")
    peripherals_c_filename = os.path.join(output_directory, "peripherals_conf.c")
    peripherals_h_filename = os.path.join(output_directory, "peripherals_conf.h")

    C_FILE_HEADER_TEMPLATE = f"""/**
 * @file    peripherals_conf.c
 * @brief   Source file containing peripherals information
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

/******************************* Include Files *******************************/

#include "drv/peripherals.h"
#include "conf/peripherals_conf.h"

/***************************** Macros Definitions ****************************/

/*************************** Variables Declarations **************************/\n
"""
    HEADER_FILE_HEADER_TEMPLATE = f"""/**
 * @file    peripherals_conf.h
 * @brief   Header file containing peripherals information
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2025
 */

#ifndef PERIPHERALS_CONF_H
#define PERIPHERALS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_PERIPHERALS {{nb_peripherals}}u

{{defines}}

#endif /* PERIPHERALS_CONF_H */
"""
    defines = []
    desc_table_entries = []
    conf_table_entries = []
    instances = []
    mutex_queue_definitions = []
    peripherals_list = []

    def generate_define_value(periph, index):
        return f"#define {periph.upper()} {index}u"
    def generate_desc_table_entry(periph):
        return f"    {{ .p_instance = &{periph.lower()}_inst }},"
    def generate_conf_table_entry(periph, p_type, p_synchro, p_flow_type):
        return (f"    {{ .peripheral = {ref}, .type = PERIPHERAL_{p_type.upper()}, .synchronisation = PERIPHERAL_{p_synchro.upper()}, "
                f".flow_type = PERIPHERAL_{p_flow_type.upper()}, .p_mutex_queue = &{periph.lower()}_mutex_queue, "
                f".p_rx_mutex_queue = &{periph.lower()}_rx_mutex_queue, .p_tx_mutex_queue = &{periph.lower()}_tx_mutex_queue }},")
    def generate_c_instance(periph, p_type, params):
        instance_name = f"{periph.lower()}_inst"
        struct_name = f"{p_type.lower()}Inst_t"
        params_str = "\n".join([f"    .{param} = {value}," for param, value in params.items()])
        return f"""
/**
 * @var     {instance_name}
 * @brief   {periph.lower()} instance declaration
 */
static {struct_name} {instance_name} = {{
{params_str}
}};
"""
    def generate_mutex_queue_definition(periph):
        return f"""
/**
 * @var     {periph.lower()}_mutex_queue
 * @brief   Mutex queue for {periph}
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_mutex_queue = {{0}};

/**
 * @var     {periph.lower()}_rx_mutex_queue
 * @brief   Mutex queue for {periph} reception
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_rx_mutex_queue = {{0}};

/**
 * @var     {periph.lower()}_tx_mutex_queue
 * @brief   Mutex queue for {periph} transmission
 */
static mutexQueue_t IN_MUTEX_QUEUE_SECTION {periph.lower()}_tx_mutex_queue = {{0}};
"""
    def generate_variable_declarations(peripherals_info):
        instance_declarations = []
        mutex_declarations = []
        for periph, p_type in peripherals_info:
            instance_name = f"{periph.lower()}_inst"
            struct_name = f"{p_type.lower()}Inst_t"
            instance_declarations.append(f"static {struct_name} {instance_name};\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_mutex_queue;\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_rx_mutex_queue;\n")
            mutex_declarations.append(f"static mutexQueue_t {periph.lower()}_tx_mutex_queue;\n")
        return instance_declarations, mutex_declarations

    for index, periph in enumerate(peripherals, start=1):
        ref = periph["ref"]
        p_type = periph["type"]
        p_synchro = periph["synchronisation"]
        p_flow_type = periph["flow_type"]
        defines.append(generate_define_value(ref, index))
        desc_table_entries.append(generate_desc_table_entry(ref))
        conf_table_entries.append(generate_conf_table_entry(ref, p_type, p_synchro, p_flow_type))
        # For additional parameters, we take all the keys other than ref,type,mode,flow
        params = {}
        for key, value in periph.items():
            if key not in ["ref", "type", "synchronisation", "flow_type"]:
                params[key] = value
        instances.append(generate_c_instance(ref, p_type, params))
        mutex_queue_definitions.append(generate_mutex_queue_definition(ref))
        peripherals_list.append((ref, p_type))
    instance_declarations, mutex_declarations = generate_variable_declarations(peripherals_list)

    c_content = C_FILE_HEADER_TEMPLATE
    c_content += "".join(instance_declarations) + "\n"
    c_content += "".join(mutex_declarations) + "\n"
    c_content += """
/*************************** Variables Definitions ***************************/

/**
 * @var     g_peripherals_conf_table
 * @brief   Configuration table where all peripherals configurations are stored
 */
peripheralConf_t IN_CONF_TABLES_SECTION g_peripherals_conf_table[CONFIG_MAX_NB_PERIPHERALS] =
{
"""
    c_content += "\n".join(conf_table_entries)
    c_content += "\n};\n"
    c_content += """
/**
 * @var     g_peripherals_desc_table
 * @brief   Configuration table where all peripherals descriptors are stored
 */
peripheralDesc_t IN_DESC_TABLES_SECTION g_peripherals_desc_table[CONFIG_MAX_NB_PERIPHERALS] =
{
"""
    c_content += "\n".join(desc_table_entries)
    c_content += "\n};\n"
    c_content += "".join(instances)
    c_content += "".join(mutex_queue_definitions)

    h_content = HEADER_FILE_HEADER_TEMPLATE.replace("{nb_peripherals}", str(len(peripherals)))
    h_content = h_content.replace("{defines}", "\n".join(defines))

    with open(peripherals_c_filename, "w") as f:
        f.write(c_content)
    with open(peripherals_h_filename, "w") as f:
        f.write(h_content)

# ==============================================================================
# ================================ Main Function ===============================
# ==============================================================================
def main():
    parser = argparse.ArgumentParser(
        description="Génère les fichiers de configuration C/H du système embarqué à partir d'un fichier JSON unique."
    )
    parser.add_argument("-i", "--input", required=True, help="Chemin vers le fichier JSON d'entrée")
    parser.add_argument("-o", "--output", required=True, help="Dossier de destination des fichiers générés")
    args = parser.parse_args()

    if not os.path.exists(args.output):
        os.makedirs(args.output)

    with open(args.input, "r") as f:
        data = json.load(f)

    system = data.get("bsp", {})

    if "peripherals" in system:
        generate_peripherals_conf(system["peripherals"], args.output)

if __name__ == "__main__":
    main()
