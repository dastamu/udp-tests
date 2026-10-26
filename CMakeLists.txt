cmake_minimum_required(VERSION 3.15)
project(Udp1Project LANGUAGES CXX)

# Force clangd to generate a build database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configuring the C++20 standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Definition of executable and sources
add_executable(udp1 src/udp1.cpp)

# Optimization and warning flags for GCC/Clang
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(udp1 PRIVATE -O3 -Wall -Wextra)
endif()

# Rename output file to udp.exe (optional, useful on Windows)
#set_target_properties(udp_app PROPERTIES OUTPUT_NAME "udp" SUFFIX ".exe")
