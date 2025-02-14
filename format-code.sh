#!/bin/bash

# List of directories to ignore during formatting
ignored_dirs=(".vscode" "build" "tools")

# Get all directories at depth 1 in the current directory
directories=$(find . -mindepth 1 -maxdepth 1 -type d)

# Check if clang-format is installed and get its version
if ! command -v clang-format &> /dev/null; then
    echo "Error: clang-format is not installed. Please install it before running this script."
    exit 1
fi

# Get clang-format version
clang_version=$(clang-format --version | grep -oE '[0-9]+' | head -1)

# Ensure the correct version (change this if a specific version is required)
required_version=19
if [[ "$clang_version" -lt "$required_version" ]]; then
    echo "Error: clang-format version $required_version or higher is required. Found version $clang_version."
    exit 1
fi

# Loop through all directories
for dir in $directories; do
    ignore=false

    # Check if the directory is in the ignored list
    for ignored_dir in "${ignored_dirs[@]}"; do
        if [[ "$dir" == *"$ignored_dir"* ]]; then
            ignore=true
            break
        fi
    done

    # Skip ignored directories
    if $ignore; then
        continue
    fi

    # Apply clang-format to C and header files, excluding *.ld.h files
    if [ -d "$dir" ]; then
        echo "Formatting files in directory: $dir"
        find "$dir" -type f \( -name "*.c" -o -name "*.h" \) ! -name "*.ld.h" -exec clang-format -i --verbose {} +
    else
        echo "Warning: Directory $dir does not exist. Skipping..."
    fi
done

echo "Clang-format check completed."
