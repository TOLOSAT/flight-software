# Flight Software Dockerfile

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="1.0"
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
        curl \
        bash-completion \
        sudo \
        ca-certificates

# Install clang-format-19
RUN echo "deb http://apt.llvm.org/jammy/ llvm-toolchain-jammy-19 main" | tee /etc/apt/sources.list.d/llvm.list && \
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/llvm.asc && \
    apt-get update && \
    apt-get install -y clang-format-19 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-19 100

# Git autocomplete
RUN curl -o /etc/bash_completion.d/git-completion.bash https://raw.githubusercontent.com/git/git/master/contrib/completion/git-completion.bash

# Makefile target autocomplete
RUN echo '_make_target_completion() {' > /etc/bash_completion.d/make && \
    echo '  local cur_word targets' >> /etc/bash_completion.d/make && \
    echo '  cur_word="${COMP_WORDS[COMP_CWORD]}"' >> /etc/bash_completion.d/make && \
    echo '  targets=$(make -qp 2>/dev/null | awk -F: '\''/^[a-zA-Z0-9][^$#\/\t=]*:/ {print $1}'\'' | sort -u)' >> /etc/bash_completion.d/make && \
    echo '  COMPREPLY=($(compgen -W "${targets}" -- "${cur_word}"))' >> /etc/bash_completion.d/make && \
    echo '}' >> /etc/bash_completion.d/make && \
    echo 'complete -F _make_target_completion make' >> /etc/bash_completion.d/make

# Clean packets
RUN apt-get -y autoremove && apt-get -y clean

# GDB symlink
RUN ln -s /usr/bin/gdb-multiarch /usr/bin/arm-none-eabi-gdb

# Create user
RUN useradd -ms /bin/bash tapas && \
    echo 'tapas:password' | chpasswd && \
    echo 'tapas ALL=(ALL) NOPASSWD: ALL' >> /etc/sudoers

# Set up bashrc properly
RUN echo 'if [ -f /usr/share/bash-completion/bash_completion ]; then' >> /home/tapas/.bashrc && \
    echo '  . /usr/share/bash-completion/bash_completion' >> /home/tapas/.bashrc && \
    echo 'fi' >> /home/tapas/.bashrc

# Switch to the new user
USER tapas

# Create Volume where the repo will be mounted
WORKDIR /tmp/flight-software

# Just to know if it is a docker
ENV DOCKER_WARNING no

# Start a login shell to load .bashrc properly
CMD ["/bin/bash", "--login"]
