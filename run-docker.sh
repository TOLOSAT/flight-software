#!/bin/bash

# Check if the Docker image exists
if [[ "$(docker image ls -q tolosat-devtool 2> /dev/null)" == "" ]]; then
    echo "Docker image 'tolosat-devtool' does not exist, building..."
    # Build the Docker image from the Dockerfile in the current directory
    docker build -t tolosat-devtool .
    if [ $? -eq 0 ]; then
        echo "Docker image 'flight-software' built successfully."
        # Run the Docker container after the image is built
        sleep 1
        echo "Launching the docker ..."
        docker run -it -d --rm --name tolosat-docker --hostname tolosat-devtool -e DISPLAY=$DISPLAY -v $(pwd):/home/tapas/flight-software --privileged -v /dev/bus/usb:/dev/bus/usb tolosat-devtool:latest
    else
        echo "Failed to build Docker image 'tolosat-devtool'."
        exit 1
    fi
else
    echo "Docker image 'tolosat-devtool' already exists."
    # Run the Docker container if the image already exists
    sleep 1
    echo "Launching the docker ..."
    docker run -it -d --rm --name tolosat-docker --hostname tolosat-devtool -e DISPLAY=$DISPLAY -v $(pwd):/home/tapas/flight-software --privileged -v /dev/bus/usb:/dev/bus/usb tolosat-devtool:latest
fi
