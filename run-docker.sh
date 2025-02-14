#!/bin/bash

CONTAINER_NAME="tapas-container"
IMAGE_NAME="tolosat-devtool"
RUN_OPTION="-dit"  # By default, run in detached mode

# Show help message
show_help() {
    echo "TAPAS DOCKER RUN SCRIPT"
    echo "This script is designed to automatically handle the creation, running,"
    echo "and updating of the Docker container based on the Dockerfile."
    echo "Available options:"
    echo "  -h, --help     : Show this help message."
    echo "  -a, --attach   : Attach to the Docker container (create one if it doesn't exist)."
    echo "  -k, --kill     : Kill the running container."
    echo "  --update       : Update the Docker image from the Dockerfile."
    exit 0
}

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

# Parse options
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            ;;
        -a|--attach)
            RUN_OPTION="-it"  # Run in interactive mode if attach is specified
            shift # past argument
            ;;
        -k|--kill)
            KILL=true
            shift # past argument
            ;;
        --update)
            build_docker_image
            exit 0
            ;;
        -*)
            echo "Unknown option $1"
            show_help
            ;;
        *)
            POSITIONAL_ARGS+=("$1") # save positional arg
            shift # past argument
            ;;
    esac
done

# Restore positional parameters
set -- "${POSITIONAL_ARGS[@]}"

# Check if the Docker image exists
if [[ "$(docker image ls -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
    echo "Docker image '$IMAGE_NAME' does not exist, building..."
    build_docker_image
else
    echo "Docker image '$IMAGE_NAME' exists."
fi

# Get the container ID if the container is already running
RUNNING_CONTAINER=$(docker ps -q -f name=$CONTAINER_NAME)

# If -k or --kill option is used, kill the running container
if [[ "$KILL" == true ]]; then
    if [[ "$RUNNING_CONTAINER" != "" ]]; then
        echo "Killing the container '$CONTAINER_NAME'"
        docker kill $CONTAINER_NAME > /dev/null
        exit 0
    else
        echo "Container '$CONTAINER_NAME' is not running."
        exit 1
    fi
fi

# Launch the container with the appropriate options (either detached or attached mode)
if [[ "$RUNNING_CONTAINER" == "" ]]; then
    echo "Launching the container '$CONTAINER_NAME'."
    docker run $RUN_OPTION --user root --rm --name $CONTAINER_NAME --hostname $CONTAINER_NAME --net=host -v $(pwd):/tmp/$(basename $(pwd)) $IMAGE_NAME:latest
else
    if [[ "$RUN_OPTION" == "-it" ]]; then
        echo "Container '$CONTAINER_NAME' is already running, attaching."
        docker attach $CONTAINER_NAME
    else
        echo "Container '$CONTAINER_NAME' is already running."
    fi
fi
