# Flight Software Dockerfile

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="0.17"
LABEL description="Docker for TOLOSAT Autonomous Payload & Avionic Software (TAPAS)"

# Fancier prompt
ENV color_prompt=yes

# Tools Installation
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
        build-essential \
        cppcheck \
        doxygen \
        gcc-arm-none-eabi \
        gdb-multiarch \
        git \
        graphviz \
        kconfig-frontends \
        nano \
        picocom \
        qemu-system \
        telnet \
        vim \
        wget \
        curl

# Install clang-format-19
RUN echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main" | tee /etc/apt/sources.list.d/llvm.list && \
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/llvm.asc && \
    apt-get update && \
    apt-get install -y clang-format-19 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-19 100

# Clean packets
RUN apt-get -y autoremove
RUN apt-get -y clean

# Create symbolic link for arm-none-eabi-gdb to gdb-multiarch
RUN ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

# Create a new user
RUN useradd -ms /bin/bash tapas
RUN echo 'tapas:password' | chpasswd
RUN echo 'tapas ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

# Switch to the new user
USER tapas

# Create Volume where the repo will be mounted
WORKDIR /tmp/flight-software

# Just to know if it is a docker
ENV DOCKER_WARNING no

# Start a shell session as the new user
CMD ["/bin/bash"]
