#!/bin/bash

# Checking if first argument exists
if [ -z "$1" ]; then
    echo " Error: give an argument, one of -gcc|-make|-cmake|-meson !!!"
    exit 1
fi

StartStamp=$(date +%s%N)

# Checking first argument
case "$1" in
    "-gcc")
        echo " GCC..."
        mkdir -p build-gcc
        g++ -std=c++20 -O3 src/udp1.cpp -o build-gcc/udp1.exe
        ;;
    "-make")
        echo " Make..."
        ;;
    "-cmake")
        echo " Cmake..."
        ;;
    "-meson")
        echo " Meson..."
        ;;
    *)
        printf " Unknown option: '${1}'\n Use: sh ${0} -gcc|-make|-cmake|-meson"
        exit 1
        ;;
esac

StopStamp=$(date +%s%N)
ms=$(( (StopStamp - StartStamp) / 1000000 ))
echo " -- Duration: $ms [ms]"

# end