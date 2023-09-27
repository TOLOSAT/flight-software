# Flight Software Dockerfile 

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="0.5"
LABEL description="Docker for TOLOSAT Autonomous Payload & Avionic Software (TAPAS)"

# Fancier prompt
ENV color_prompt=yes

# Tools Installation
RUN apt update && apt upgrade -y
RUN apt install -y vim build-essential git

# Toolchain Installation
RUN apt install -y gcc-arm-none-eabi
RUN apt install -y gdb-multiarch

# OpenOCD Installation
RUN apt install -y openocd

# CPPCheck Installation
RUN apt install -y cppcheck

# Doxygen Installation
RUN apt install -y doxygen

# Create a new user
RUN useradd -ms /bin/bash tapas
RUN echo 'tapas:password' | chpasswd
RUN echo 'tapas ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers
USER tapas

# Create Volume where the repo will be mounted
WORKDIR /home/tapas/flight-software
VOLUME /home/tapas/flight-software

# Just to know if it is a docker
ENV IS_A_DOCKER yes

# Start a shell session as the new user
CMD ["/bin/bash"]