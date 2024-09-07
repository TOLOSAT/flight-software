#!/bin/bash

CONTAINER_NAME="tapas-container"
IMAGE_NAME="tolosat-devtool"

# Show help message if -h is called
if [[ "$*" == *"-h"* ]]; then
    echo "TAPAS DOCKER RUN SCRIPT"
    echo "This script is designed to automatically handle the creation, running,"
    echo "and updating of the Docker container based on the Dockerfile."
    echo "Available options:"
    echo "  -h : Show this help message."
    echo "  -a : Attach to the Docker container (create one if it doesn't exist)."
    echo "  -k : Kill the running container."
    echo "  -u : Update the Docker image from the Dockerfile."
    exit 0
fi

# Function to build or rebuild the Docker image
build_docker_image() {
    echo "Building Docker image '$IMAGE_NAME' from Dockerfile..."
    docker build -t $IMAGE_NAME .
    if [ $? -eq 0 ]; then
        echo "Docker image '$IMAGE_NAME' built successfully."
    else
        echo "Failed to build Docker image '$IMAGE_NAME'."
        exit 1
    fi
}

# If -u option is used, update the Docker image
if [[ "$*" == *"-u"* ]]; then
    build_docker_image
    exit 0
fi

# Check if the Docker image exists
if [[ "$(docker image ls -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Docker image '$IMAGE_NAME' does not exist, building..."
    build_docker_image
else
    echo "Docker image '$IMAGE_NAME' exists."
fi

# Get the container ID if the container is already running
RUNNING_CONTAINER=$(docker ps -q -f name=$CONTAINER_NAME)

# If -k option is used, kill the running container
if [[ "$*" == *"-k"* ]]; then
    if [[ "$RUNNING_CONTAINER" != "" ]]; then
        echo "Killing the container '$CONTAINER_NAME'"
        docker kill $CONTAINER_NAME > /dev/null
        exit 0
    else
        echo "Container '$CONTAINER_NAME' is not running."
        exit 1
    fi
fi

# If -a option is used, attach to the container
if [[ "$*" == *"-a"* ]]; then
    if [[ "$RUNNING_CONTAINER" != "" ]]; then
        echo "Container '$CONTAINER_NAME' is already running, attaching."
        docker attach $CONTAINER_NAME
    else
        echo "Launching the container '$CONTAINER_NAME'."
        docker run -it --rm --name $CONTAINER_NAME --hostname $CONTAINER_NAME --net=host -v $(pwd):/tmp/$(basename $(pwd)) $IMAGE_NAME:latest
    fi
    exit 0
fi

# If no specific option is used, launch the container in the background
echo "Launching the container '$CONTAINER_NAME' in the background."
docker run -dit --rm --name $CONTAINER_NAME --hostname $CONTAINER_NAME --net=host -v $(pwd):/tmp/$(basename $(pwd)) $IMAGE_NAME:latest > /dev/null
