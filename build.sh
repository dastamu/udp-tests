#!/bin/bash

# Checking if first argument exists
if [ -z "$1" ]; then
    echo " Error: give an argument, one of -gcc|-make|-makeclean|-cmake|-meson|-mesonclean !!!"
    exit 1
fi

StartStamp=$(date +%s%N)

# Checking first argument
case "$1" in
    "-gcc")
        echo "=== GCC..."
        mkdir -p "build-gcc"
        g++ -std=c++20 -O3 src/udp1.cpp -o build-gcc/udp1.exe
        ;;
    "-make")
        echo "=== Make..."
        make
        ;;
    "-makeclean")
        echo "=== Make Clean All..."
        make clean all
        ;;
    "-cmake")
        echo "=== Cmake..."
        #cmake -B "build-cmake" -S . && cmake --build "build-cmake"
        cmake -G Ninja -B "build-cmake" -S . && cmake --build "build-cmake"
        # (For clangd) Create a symbolic link or copy the generated file to the main directory
        # On Linux/macOS:
        #ln -s build/compile_commands.json .
        # On Windows (cmd as Administrator):
        #mklink compile_commands.json build\compile_commands.json
        ;;
    "-meson")
        echo "=== Meson..."
        meson setup "build-meson" -Dbuildtype=release && meson compile -C "build-meson"
        ;;
    "-mesonclean")
        echo "=== Meson..."
        meson compile -C "build-meson" --clean
        meson setup "build-meson" --buildtype=release && meson compile -C "build-meson"
        ;;
    *)
        printf "=== Unknown option: '${1}'\nUse: sh ${0} -gcc|-make|-makeclean|-cmake|-meson|-mesonclean\n"
        exit 1
        ;;
esac

StopStamp=$(date +%s%N)
ms=$(( (StopStamp - StartStamp) / 1000000 ))
echo "=== Duration: $ms [ms]"

# end