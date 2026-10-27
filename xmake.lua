target("udp1")
    set_kind("binary")
    add_files("src/udp1.cpp")
    set_languages("c++20")
    set_optimize("fastest") -- odpowiednik -O3
    -- Katalog budowania
    set_targetdir("build-xmake")
    -- Wymuszenie własnej nazwy pliku wykonywalnego na dysku
    set_filename("udp1.exe")
    -- Automatyczne generowanie bazy danych kompilacji po każdym zbudowaniu projektu
    after_build(function (target)
        import("core.project.project")
        -- Generuje compile_commands.json w katalogu zdefiniowanym w set_targetdir
        project.export("compile_commands", path.join(target:targetdir(), "compile_commands.json"))
    end)
