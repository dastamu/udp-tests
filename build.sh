#!/bin/bash

# Checking first argument
case "$1" in
    "-gcc")
        echo " GCC..."
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

# end