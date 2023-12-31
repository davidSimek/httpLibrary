#!/bin/bash

# -------- 
# LOGGING
# --------

REDF="\e[31m"
GREENF="\e[32m"
BLUEF="\e[34m"
ENDCOLOR="\e[0m"

info() {
    echo -e "${BLUEF}[INFO] ${ENDCOLOR}$1"
}
success() {
    echo -e "${GREENF}[SUCCESS] ${ENDCOLOR}$1"
}
error() {
    echo -e "${REDF}[ERROR] ${ENDCOLOR}$1"
}

# ---------
# BUILDING
# ---------

# WINDOWS
build_windows() {
    info "Building for Windows"
    logFile="logFile.log"

    if [[ $1 = "-g" ]]; then
        info "building with debug flags"
        x86_64-w64-mingw32-gcc -g -c -DWINDOWS=1 -o out/httpLibrary.lib src/httpLibrary.c -lws2_32 2> "$logFile"
    else
        x86_64-w64-mingw32-gcc -c -DWINDOWS=1 -o out/httpLibrary.lib src/httpLibrary.c -lws2_32 2> "$logFile"
    fi

    if [ $? -eq 0 ]; then
        success "Program successfully compiled."
    else
        error "Program didn't compile."
        cat $logFile
    fi

}

# GNU/LINUX
build_linux() {
    info "Building for GNU/Linux"
    logFile="logFile.log"

    if [[ $1 = "-g" ]]; then
        info "building with debug flags"
        gcc -g -c src/httpLibrary.c -o out/httpLibrary.o 2> "$logFile"
        ar rcs out/httpLibrary.a out/httpLibrary.o 2>> "$logFile"

        rm out/httpLibrary.o
    else
        gcc -c src/httpLibrary.c -o out/httpLibrary.o 2> "$logFile"

        ar rcs out/httpLibrary.a out/httpLibrary.o 2>> "$logFile"

        rm out/httpLibrary.o
    fi


    if [ $? -eq 0 ]; then
        success "Program successfully compiled."
    else
        error "Program didn't compile."
        cat $logFile
    fi
}

# -----
# MAIN
# -----

if [ "$1" = "-w" ]; then
    build_windows $2
elif [[ "$1" = "-l" ]]; then
    build_linux $2
else
    error "Usage: build [-w(Windows)|-l(GNU/Linux] -g(?)"
fi
