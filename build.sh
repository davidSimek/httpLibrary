#!/bin/bash

# WINDOWS
build_windows() {
    echo "building for Windows"
    if [[ $1 = "g" ]]; then
        echo "building with debug flags"
    fi
}

# GNU/LINUX
build_linux() {
    echo "building for Windows"
    if [[ $1 = "g" ]]; then
        echo "building with debug flags"
    fi
}

if [ "$1" = "-w" ]; then
    windows_function
else
    other_function
fi

