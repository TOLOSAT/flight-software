# Flight Software Dockerfile 

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="0.8"
LABEL description="Docker for TOLOSAT Autonomous Payload & Avionic Software (TAPAS)"

# Fancier prompt
ENV color_prompt=yes

# Tools Installation
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y \
        build-essential \
        cppcheck \
        dialog \
        doxygen \
        gdb-multiarch \
        gcc-arm-none-eabi \
        git \
        nano \
        openocd \
        telnet \
        vim
RUN apt-get -y autoremove
RUN apt-get -y clean

# Create a new user
RUN useradd -ms /bin/bash tapas
RUN echo 'tapas:password' | chpasswd
RUN echo 'tapas ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers
USER tapas

# Create Volume where the repo will be mounted
WORKDIR /home/tapas/flight-software
VOLUME /home/tapas/flight-software

# Just to know if it is a docker
ENV DOCKER_WARNING no

# Start a shell session as the new user
CMD ["/bin/bash"]