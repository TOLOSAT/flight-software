#!/usr/bin/env python3

import argparse
import os
from datetime import datetime

def parse_config(config_file, output_dir):
    # Define the output header file
    output_file = os.path.join(output_dir, 'autoconf.h')

    # Get current date for the header
    current_date = datetime.now().strftime("%d/%m/%Y")

    # Open the .config file and the output header file
    with open(config_file, 'r') as config, open(output_file, 'w') as header:
        # Write the header comment
        header.write(f"""/**
 * @file    autoconf.h
 * @brief   Header file for buffer configuration
 * @author  Auto-generated
 * @date    {current_date}
 * 
 * @copyright Copyright (c) TOLOSAT 2024
 */

#ifndef AUTOCONF_H
#define AUTOCONF_H

""")

        # Parse each line of the .config file
        for line in config:
            line = line.strip()

            # Ignore empty lines
            if not line:
                continue

            # Ignore comments in the .config file
            if line.startswith("#"):
                if "is not set" in line:
                    # Handle disabled config options
                    option = line.split()[1]
                    header.write(f"// {option} is not set\n")
                continue

            # Handle config options that are set
            if "=" in line:
                option, value = line.split("=", 1)
                option = option.strip()
                value = value.strip()

                if value == "y":
                    header.write(f"#define {option} y\n")
                elif value == "n":
                    header.write(f"// {option} is not set\n")
                else:
                    header.write(f"#define {option} {value}\n")

        # Write the footer comment
        header.write("\n#endif /* AUTOCONF_H */\n")

def main():
    # Set up argument parsing
    parser = argparse.ArgumentParser(description="Generate autoconf.h from a .config file.")
    parser.add_argument('-i', '--input', required=True, help="Path to the input .config file")
    parser.add_argument('-o', '--output', required=True, help="Path to the output directory")

    args = parser.parse_args()

    # Ensure output directory exists
    if not os.path.exists(args.output):
        os.makedirs(args.output)

    # Call the parsing function
    parse_config(args.input, args.output)

if __name__ == "__main__":
    main()
