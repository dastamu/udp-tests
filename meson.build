project('udp1_project', 'cpp',
  version : '0.1',
  default_options : [
    'cpp_std=c++20',
    'optimization=3',
    'warning_level=3', # Automatycznie włącza flagi -Wall oraz -Wextra
    'werror=false'
  ]
)

# Definicja flag kompilatora potrzebnych dla gniazd POSIX (Cygwin/MSYS2)
cpp_compiler = meson.get_compiler('cpp')
add_project_arguments('-D_GNU_SOURCE', language : 'cpp')

# Główny plik wykonywalny projektu (wygeneruje udp.exe na Windowsie lub udp na Linuksie)
executable('udp1',
  'src/udp1.cpp',
  #strip : true, # Usuwa symbole debugowania i zmniejsza plik wykonywalny
  install : true
)
