target("udp1")
    set_kind("binary")
    add_files("src/udp1.cpp")
    set_languages("c++20")
    set_optimize("fastest") -- odpowiednik -O3
