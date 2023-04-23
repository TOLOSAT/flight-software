#!/bin/bash

# Check if the Docker image exists
if [[ "$(docker images -q flight-software 2> /dev/null)" == "" ]]; then
    echo "Docker image 'flight-software' does not exist, building..."
    # Build the Docker image from the Dockerfile in the current directory
    docker build -t flight-software .
    if [ $? -eq 0 ]; then
        echo "Docker image 'flight-software' built successfully."
        # Run the Docker container after the image is built
        docker run -it --rm --hostname tolosat-devtool -p 4444:4444 -v $(pwd):/app --privileged -v /dev/bus/usb:/dev/bus/usb flight-software:latest
    else
        echo "Failed to build Docker image 'flight-software'."
        exit 1
    fi
else
    echo "Docker image 'flight-software' already exists."
    # Run the Docker container if the image already exists
    docker run -it --rm --hostname tolosat-devtool -p 4444:4444 -v $(pwd):/app --privileged -v /dev/bus/usb:/dev/bus/usb flight-software:latest
fi



