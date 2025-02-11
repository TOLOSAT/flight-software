#!/bin/bash

ignored_dirs=(".vscode" "build" "tools")

directories=$(find . -mindepth 1 -maxdepth 1 -type d)

for dir in $directories; do
    ignore=false
    for ignored_dir in "${ignored_dirs[@]}"; do
        if [[ "$dir" == *"$ignored_dir"* ]]; then
            ignore=true
            break
        fi
    done
    if $ignore; then
        continue
    fi
    if [ -d "$dir" ]; then
        find "$dir" -type f \( -name "*.c" -o -name "*.h" \) ! -name "*.ld.h" -exec clang-format -i {} +
    else
        echo "Warning: Directory $dir does not exist. Skipping..."
    fi
done
