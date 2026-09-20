# Copyright (c) TOLOSAT 2026
# SPDX-License-Identifier: Apache-2.0

# Flight Software Dockerfile

# Base Image
FROM ubuntu:22.04

# Labels
LABEL version="1.3"
LABEL description="Development environment for the TOLOSAT Flight Software"

# Fancier prompt
ENV color_prompt=yes

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Use bash with pipefail for safer multi-command RUN instructions
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# Tools Installation
RUN apt-get update
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

# Verify installed versions (fail early if unexpected)
RUN cppcheck --version | grep -E '^Cppcheck 2\.7(\.|$)' && \
    arm-none-eabi-gcc --version | head -n 1 | grep -E '10\.3'

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

# Shorten the prompt to show paths relative to the mounted workspace
RUN cat <<'EOF' >> /home/tapas/.bashrc
__tapas_prompt_relpath() {
  local ws="${TAPAS_WORKSPACE%/}"
  local wd="${PWD%/}"
  if [[ -n "$ws" && "$wd" == "$ws"* ]]; then
    if [[ "$wd" == "$ws" ]]; then
      printf '%s' "${ws##*/}"
    else
      printf '%s/%s' "${ws##*/}" "${wd#${ws}/}"
    fi
    return
  fi
  local home="${HOME%/}"
  if [[ -n "$home" && "$wd" == "$home"* ]]; then
    if [[ "$wd" == "$home" ]]; then
      printf '~'
    else
      printf '~/%s' "${wd#${home}/}"
    fi
  else
    printf '%s' "$PWD"
  fi
}

__tapas_prompt_use_color() {
  if [[ -n "${TAPAS_COLOR_PROMPT:-}" ]]; then
    [[ "${TAPAS_COLOR_PROMPT}" = yes ]]
    return
  fi
  if [[ -t 1 ]] && command -v tput >/dev/null 2>&1 && tput setaf 1 >/dev/null 2>&1; then
    return 0
  fi
  return 1
}

if __tapas_prompt_use_color; then
  PS1='${debian_chroot:+($debian_chroot)}\[\033[01;32m\]\u@\h\[\033[00m\]:\[\033[01;36m\]$(__tapas_prompt_relpath)\[\033[00m\]\$ '
else
  PS1='${debian_chroot:+($debian_chroot)}\u@\h:$(__tapas_prompt_relpath)\$ '
fi
EOF

RUN chown -R tapas:tapas /home/tapas

# Switch to the new user
USER tapas

# Default working directory (real workspace mount overrides this)
WORKDIR /home/tapas

# Just to know if it is a docker
ENV DOCKER_WARNING=no

# Start a login shell to load .bashrc properly
CMD ["/bin/bash", "--login"]
