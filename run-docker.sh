#!/bin/bash

CONTAINER_NAME="tapas-container"
IMAGE_NAME="tolosat-devtool"

# First show the help message if -h is called
if [[ "$*" == *"-h"* ]]; then
    echo "TAPAS DOCKER RUN SCRIPT"
    echo "This script has been made to automatically create the"
    echo "docker based on the dockerfile"
    echo "  -h : Show this message"
    echo "  -a : Attach to the docker (create one if necessary)"
    exit 0
fi

# Check if the docker image exists
if [[ "$(docker image ls -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Docker image '$IMAGE_NAME' does not exist, building ..."
    # Build the docker image from the dockerfile
    docker build -t $IMAGE_NAME .
    if [ $? -eq 0 ]; then
        echo "Docker image '$IMAGE_NAME' built successfully."
        sleep 1
    else
        echo "Failed to build docker image '$IMAGE_NAME'."
        exit 1
    fi
else
    echo "Docker image '$IMAGE_NAME' exists."
    sleep 1
fi

# Get the container ID if the container is already running
RUNNING_CONTAINER=$(docker ps -q -f name=$CONTAINER_NAME)

# Check if we want to kill the container
if [[ "$*" == *"-k"* ]]; then
    if [[ "$RUNNING_CONTAINER" != "" ]]; then
        echo "Killing the container '$CONTAINER_NAME'"
        docker kill $CONTAINER_NAME > /dev/null
        exit 0
    else
        echo "Container '$CONTAINER_NAME' does not exist"
        exit -1
    fi
fi

# Check if we want to attach the container
if [[ "$*" == *"-a"* ]]; then
    if [[ "$RUNNING_CONTAINER" != "" ]]; then
        echo "Container '$CONTAINER_NAME' is already running, attaching"
        docker attach $CONTAINER_NAME
    else
        echo "Launching the container '$CONTAINER_NAME'"
        docker run -it --rm --name $CONTAINER_NAME --hostname $CONTAINER_NAME --net=host -v $(pwd):/tmp/$(basename $(pwd)) $IMAGE_NAME:latest
    fi
    exit 0
fi

# Otherwise launch the container in the background
echo "Launching the container '$CONTAINER_NAME' in the background"
docker run -dit --rm --name $CONTAINER_NAME --hostname $CONTAINER_NAME --net=host -v $(pwd):/tmp/$(basename $(pwd)) $IMAGE_NAME:latest > /dev/null
