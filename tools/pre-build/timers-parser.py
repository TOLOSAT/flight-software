import csv
from datetime import datetime
import argparse
import os

# Argument parser configuration
parser = argparse.ArgumentParser(
    description="Generates timers_conf.c and timers_conf.h files from a CSV file."
)
parser.add_argument("-i", "--input", type=str, help="Path to input CSV file.")
parser.add_argument(
    "-o", "--output", type=str, help="Destination folder for generated files."
)

args = parser.parse_args()

# Check for required arguments
if not args.input or not args.output:
    parser.print_help()
    exit()

csv_file_name = args.input
output_directory = args.output

# Create output directory if it doesn't exist
if not os.path.exists(output_directory):
    os.makedirs(output_directory)

c_file_name = os.path.join(output_directory, "timers_conf.c")
h_file_name = os.path.join(output_directory, "timers_conf.h")

# Get current date for the header
current_date = datetime.now().strftime("%d/%m/%Y")


def generate_timers_conf(csv_file_name, output_directory):
    timers = []

    with open(csv_file_name, mode="r", newline="") as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            timers.append(
                {
                    "Timer Ref": row["Timer Ref"].strip(),
                    "Owner Ref": row["Owner Ref"].strip(),
                }
            )

    header_c = f"""/**
 * @file    timers_conf.c
 * @brief   Source file storing configuration table for timers
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

/******************************* Include Files *******************************/

#include "core/timers.h"
#include "conf/tasks_conf.h"
#include "conf/timers_conf.h"

/***************************** Macros Definitions ****************************/

"""

    header_h = f"""/**
 * @file    timers_conf.h
 * @brief   Header file for timer configuration
 * @author  Auto-generated
 * @date    {current_date}
 *
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef TIMERS_CONF_H
#define TIMERS_CONF_H

/***************************** Macros Definitions ****************************/

#define NB_TIMERS {len(timers)}u

"""

    timer_defines = ""

    timer_static_conf_comment = """/**
 * @var     g_timers_conf
 * @brief   Configuration table where all timers' static parameters are stored
 */
"""
    timer_static_conf = (
        timer_static_conf_comment
        + "const timerConf_t IN_CONF_TABLES_SECTION g_timers_conf[NB_TIMERS] =\n{\n"
    )
    timer_dynamic_conf_comment = """/**
 * @var     g_timers_desc_table
 * @brief   Configuration table where all timers' descriptors are stored
 */
"""
    timer_dynamic_conf = (
        timer_dynamic_conf_comment
        + "timerDesc_t IN_DESC_TABLES_SECTION g_timers_desc_table[NB_TIMERS] = {0};\n"
    )

    for i, timer in enumerate(timers):
        timer_ref = timer["Timer Ref"]
        owner_ref = timer["Owner Ref"]

        # Générer les #define pour chaque timer
        timer_defines += f"#define {timer_ref} {i}u\n"
        timer_static_conf += f"    {{ {timer_ref}, {owner_ref} }},\n"

    timer_static_conf += "};\n\n"

    with open(h_file_name, "w") as h_file:
        # Write the header file content with defines instead of enum
        h_file.write(header_h + timer_defines)
        h_file.write("\n#endif /* TIMERS_CONF_H */\n")

    with open(c_file_name, "w") as c_file:
        c_file.write(header_c)  # Write the macros to the .c file
        c_file.write(
            "\n/*************************** Variables Declarations **************************/\n\n"
        )

        c_file.write(
            "\n/*************************** Variables Definitions ***************************/\n\n"
        )
        c_file.write(timer_static_conf + timer_dynamic_conf)


if __name__ == "__main__":
    generate_timers_conf(csv_file_name, output_directory)
