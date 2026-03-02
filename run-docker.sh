#!/usr/bin/env bash
set -euo pipefail

CONTAINER_NAME="tapas-container"
IMAGE_NAME="tolosat-devtool"
PLATFORM="linux/amd64"
RUN_OPTION="-dit"  # By default, run in detached mode

# Show help message
show_help() {
    echo "TAPAS DOCKER RUN SCRIPT"
    echo "This script is designed to automatically handle the creation, running,"
    echo "and updating of the Docker container based on the Dockerfile."
    echo ""
    echo "Hard rule: everything is forced to platform: ${PLATFORM}"
    echo ""
    echo "Available options:"
    echo "  -h, --help     : Show this help message."
    echo "  -a, --attach   : Attach to the Docker container (create one if it doesn't exist)."
    echo "  -k, --kill     : Kill the running container."
    echo "  --update       : Rebuild the Docker image from the Dockerfile (forced amd64)."
    exit 0
}

require_docker() {
    command -v docker >/dev/null 2>&1 || { echo "ERROR: docker not found in PATH." >&2; exit 1; }
    docker info >/dev/null 2>&1 || { echo "ERROR: docker daemon not running / not accessible." >&2; exit 1; }
    docker buildx version >/dev/null 2>&1 || { echo "ERROR: docker buildx is required (Docker Desktop provides it)." >&2; exit 1; }
}

image_arch() {
    # Prints image architecture if image exists, empty otherwise
    docker image inspect "${IMAGE_NAME}:latest" --format '{{.Architecture}}' 2>/dev/null || true
}

container_running_arch() {
    # Prints container architecture (from `docker exec uname -m`) if running, empty otherwise.
    # Note: On Docker Desktop, uname reflects the VM kernel arch; we only use this as a hint.
    if docker ps -q -f "name=^${CONTAINER_NAME}$" >/dev/null 2>&1; then
        docker exec "${CONTAINER_NAME}" sh -lc 'uname -m' 2>/dev/null || true
    fi
}

ensure_amd64_image() {
    local arch
    arch="$(image_arch)"

    if [[ -z "${arch}" ]]; then
        echo "Docker image '${IMAGE_NAME}:latest' does not exist, building for ${PLATFORM}..."
        build_docker_image
        arch="$(image_arch)"
    fi

    if [[ "${arch}" != "amd64" ]]; then
        echo "Local image '${IMAGE_NAME}:latest' is '${arch}', but we require amd64."
        echo "Rebuilding '${IMAGE_NAME}:latest' for ${PLATFORM} (this will replace the local tag)..."
        build_docker_image
        arch="$(image_arch)"
        if [[ "${arch}" != "amd64" ]]; then
            echo "ERROR: After rebuild, '${IMAGE_NAME}:latest' is still '${arch}' (expected 'amd64')." >&2
            echo "Hint: ensure x86/amd64 emulation is enabled in Docker Desktop (Apple Silicon)." >&2
            exit 1
        fi
    fi
}

# Function to build or rebuild the Docker image (FORCED amd64)
build_docker_image() {
    echo "Building Docker image '${IMAGE_NAME}:latest' from Dockerfile (platform=${PLATFORM})..."
    # --load is important so the amd64 image is available locally for `docker run`
    # --no-cache avoids accidentally reusing an arm64 cache chain
    docker buildx build --platform "${PLATFORM}" -t "${IMAGE_NAME}:latest" --load --no-cache .
    echo "Docker image '${IMAGE_NAME}:latest' built successfully."
}

# Parse options
KILL=false
while [[ $# -gt 0 ]]; do
    case "$1" in
        -h|--help)
            show_help
            ;;
        -a|--attach)
            RUN_OPTION="-it"  # Run in interactive mode if attach is specified
            shift
            ;;
        -k|--kill)
            KILL=true
            shift
            ;;
        --update)
            require_docker
            build_docker_image
            # Enforce that the tag now points to amd64 so nobody stays stuck on arm64
            ensure_amd64_image
            exit 0
            ;;
        -*)
            echo "Unknown option $1" >&2
            show_help
            ;;
        *)
            shift
            ;;
    esac
done

require_docker

# Get the container ID if the container is already running
RUNNING_CONTAINER="$(docker ps -q -f "name=^${CONTAINER_NAME}$" || true)"

# If -k or --kill option is used, kill the running container
if [[ "${KILL}" == true ]]; then
    if [[ -n "${RUNNING_CONTAINER}" ]]; then
        echo "Killing the container '${CONTAINER_NAME}'"
        docker kill "${CONTAINER_NAME}" >/dev/null
        exit 0
    else
        echo "Container '${CONTAINER_NAME}' is not running."
        exit 1
    fi
fi

# Ensure the local image tag points to amd64 (auto-fix if someone previously built arm64)
ensure_amd64_image

# Mirror the host absolute path inside Docker so debug symbols / logs match.
REPO_DIR="$(pwd -P)"
CONTAINER_WORKDIR="${TAPAS_CONTAINER_WORKDIR:-${REPO_DIR}}"

# Launch the container with the appropriate options (either detached or attached mode)
if [[ -z "${RUNNING_CONTAINER}" ]]; then
    echo "Launching the container '${CONTAINER_NAME}' (platform=${PLATFORM})."
    docker run ${RUN_OPTION} --rm \
        --platform "${PLATFORM}" \
        --name "${CONTAINER_NAME}" \
        --hostname "${CONTAINER_NAME}" \
        --net=host \
        -v "${REPO_DIR}:${CONTAINER_WORKDIR}" \
        -w "${CONTAINER_WORKDIR}" \
        -e "TAPAS_WORKSPACE=${CONTAINER_WORKDIR}" \
        "${IMAGE_NAME}:latest"
else
    if [[ "${RUN_OPTION}" == "-it" ]]; then
        echo "Container '${CONTAINER_NAME}' is already running, attaching."
        docker attach "${CONTAINER_NAME}"
    else
        echo "Container '${CONTAINER_NAME}' is already running."
    fi
fi
