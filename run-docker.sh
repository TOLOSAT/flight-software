#!/bin/bash

# First show the help message if -h is called
if [[ "$*" == *"-h"* ]]; then
    echo "TAPAS DOCKER RUN SCRIPT"
    echo "This script has been made to automatically create the"
    echo "docker based on the dockerfile"
    echo "  -h : Show this message"
    echo "  -a : Attach to the docker after starting it"
else 
    # Check if the Docker image exists
    if [[ "$(docker image ls -q tolosat-devtool 2> /dev/null)" == "" ]]; then
        echo "Docker image 'tolosat-devtool' does not exist, building ..."
        # Build the Docker image from the Dockerfile in the current directory
        docker build -t tolosat-devtool .
        if [ $? -eq 0 ]; then
            echo "Docker image 'flight-software' built successfully."
            sleep 1
        else
            echo "Failed to build Docker image 'tolosat-devtool'."
            exit 1
        fi
    else
        echo "Docker image 'tolosat-devtool' exists."
        sleep 1
    fi

    # Run the docker
    echo "Launching the docker ..."
    if [[ "$*" == *"-a"* ]]; then
        docker run -it --rm --name tapas-container --hostname tapas-container -v $(pwd):/home/tapas/flight-software --privileged -v /dev/bus/usb:/dev/bus/usb tolosat-devtool:latest
    else
        docker run -dit --rm --name tapas-container --hostname tapas-container -v $(pwd):/home/tapas/flight-software --privileged -v /dev/bus/usb:/dev/bus/usb tolosat-devtool:latest
    fi
fi