#!/bin/bash

# Check for the presence of the doxygen.log file and the html folder
if [ -e ./doc/doxygen/doxygen.log ] || [ -d ./doc/doxygen/html ]; then
    # Deletes the file and folder if they exist
    rm -rf ./doc/doxygen/doxygen.log ./doc/doxygen/html
fi

# Run the doxygen command using the Doxyfile configuration file
doxygen ./doc/doxygen/Doxyfile