# Flight Software Dockerfile 

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="0.1"
LABEL description="Docker for TOLOSAT Flight Software development"

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

# Create a new user
RUN useradd -ms /bin/bash obc
RUN echo 'obc:password' | chpasswd
RUN echo 'obc ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers
USER obc

# Create Volume where the repo will be mounted
WORKDIR /home/obc/app
VOLUME /home/obc/app

# Just to know if it is a docker
ENV IS_A_DOCKER yes

# Start a shell session as the new user
CMD ["/bin/bash"]