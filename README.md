# udp-tests
> UDP communications example apps

## Dependences
### Debian
```sh
#@ Debian 12.x
sudo apt install g++ make cmake meson ninja-build xmake
```
## Building
```sh
sh build.sh [-gcc|-make|-cmake|-meson|-xmake]
```
## Output
### Debian
```sh
#@ Debian 12.15
stat -c "%s %n" buil*/*.exe
'
23416 build-cmake/udp1.exe
23416 build-gcc/udp1.exe
23416 build-make/udp1.exe
23416 build-meson/udp1.exe
23416 build-xmake/udp1.exe
'
```

