# Flight Software Dockerfile 
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

#OpenOCD Installation
RUN apt install -y autoconf libtool pkg-config libusb-1.0-0 libusb-1.0-0-dev
RUN git clone git://git.code.sf.net/p/openocd/code /tmp/openocd-code
RUN cd /tmp/openocd-code && ./bootstrap && ./configure --enable-ftdi --enable-stlink  && make && make install
RUN cd /tmp && rm -rf openocd-code

VOLUME /app
WORKDIR /app

EXPOSE 4444