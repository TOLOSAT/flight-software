#!/bin/bash

# Check if the "build" directory exists, if not, create it
if [ ! -d ./build ]; then
    mkdir ./build
fi

# Check for the presence of the doxygen.log file and the html folder
if [ -e ./build/doxygen/doxygen.log ] || [ -d ./build/doxygen/html ]; then
    # Deletes the file and folder if they exist
    rm -rf ./build/doxygen
fi

# Run the doxygen command using the Doxyfile configuration file
doxygen ./doc/doxygen/Doxyfile
doxygen_return=$?

# Check if the documentation generation was successful
if [ $doxygen_return -eq 0 ]; then
    # Create an alias for index.html in the "build" directory
    ln ./build/doxygen/html/index.html ./build/doxygen/index.html
else
    # Print error message in red color
    echo -e "\033[0;31mDoxygen documentation generation failed!\033[0m"
fi

# Return the code of the doxygen command as the script's exit code
exit $doxygen_return
