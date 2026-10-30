-- Ustawienie globalnej reguły automatycznego odświeżania bazy dla clangd
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".build-xmake"})

target("udp1")
    set_kind("binary")
    add_files("src/udp1.cpp")
    set_languages("c++20")
    set_optimize("fastest") -- odpowiednik -O3
    -- Katalog budowania
    set_targetdir(".build-xmake")
    -- Wymuszenie własnej nazwy pliku wykonywalnego na dysku
    set_filename("udp1.exe")

target("udp0")
    set_kind("binary")
    add_files("src/udp0.c")
    set_languages("c11")
    set_optimize("fastest") -- odpowiednik -O3
    -- Katalog budowania
    set_targetdir(".build-xmake")
    -- Wymuszenie własnej nazwy pliku wykonywalnego na dysku
    set_filename("udp0.exe")