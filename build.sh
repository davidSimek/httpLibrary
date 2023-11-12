#!/bin/bash

# -------- 
# LOGGING
# --------

REDF="\e[31m"
GREENF="\e[32m"
BLUEF="\e[34m"
ENDCOLOR="\e[0m"

info() {
    echo -e "${BLUEF}[INFO]${ENDCOLOR}$1"
}
success() {
    echo -e "${GREENF}[SUCCESS]${ENDCOLOR}$1"
}
error() {
    echo -e "${REDF}[ERROR]${ENDCOLOR}$1"
}

# ---------
# BUILDING
# ---------

# WINDOWS
build_windows() {
    info "building for Windows"
    if [[ $1 = "-g" ]]; then
        info "building with debug flags"
    fi
    error "unimplemented for now"
}

# GNU/LINUX
build_linux() {
    info "building for GNU/Linux"
    if [[ $1 = "-g" ]]; then
        info "building with debug flags"
        error "unimplemented for now"
    fi
    gcc -o out/http src/main.c src/httpLibrary.c
}

# -----
# MAIN
# -----

if [ "$1" = "-w" ]; then
    build_windows $2
elif [[ "$1" = "-l" ]]; then
    build_linux $2
else
    error "usage: build [-w(Windows)|-l(GNU/Linux] -g(?)"
fi
