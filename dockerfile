# Flight Software Dockerfile 

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="0.16"
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
        openocd \
        picocom \
        qemu-system \
        telnet \
        vim
RUN apt-get -y autoremove
RUN apt-get -y clean

# Create symbolic link for arm-none-eabi-gdb to gdb-multiarch
RUN ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

# Create a new user
RUN useradd -ms /bin/bash tapas
RUN echo 'tapas:password' | chpasswd
RUN echo 'tapas ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

# Add the 'tapas' user to the 'plugdev' group
RUN usermod -aG plugdev tapas

# Switch to the new user
USER tapas

# Create Volume where the repo will be mounted
WORKDIR /tmp/flight-software

# Just to know if it is a docker
ENV DOCKER_WARNING no

# Start a shell session as the new user
CMD ["/bin/bash"]
