
# [Ter/Terlang](./README.md)
> Programming language for scripting with syntax similar to C++

![Ter/Terlang](./logo-ter-terlang.png) 

---

## Summary
1. [Building and Installing on UNIX-style systems](#building-on-unix-style-systems)
2. [Building and Installing on Windows](#building-and-installing-on-windows)
3. [Only installing on Windows or Ubuntu](#only-installing-on-windows-or-ubuntu)
4. [Using](#using)
5. [Tutorials](#tutorials)
6. [Extensions and LICENSE](#extensions-and-license)

---

### Dependencies
+ [GNU GCC](https://gcc.gnu.org/), [Clang](https://clang.llvm.org/) or [MSVC](https://learn.microsoft.com/en-us/cpp/?view=msvc-170)
+ [CMake](https://cmake.org/)
> C++23

### Building on UNIX-style systems
> macOS, GNU/Linux, BSD-Like, Haiku and others.

```bash
git clone https://github.com/terroo/terlang
cd terlang
cmake -B build .
cmake --build build
sudo cmake --install build
```

REPL:
> To test the `ter` command.
```cpp
$ ter
ter> output(args())
Ter/Terlang VERSION: 0.0.1
ter> exit
```

---

### Building and Installing on Windows
With MSVC!
> Open PowerShell (Run as Administrator) from the Windows Start Menu

```bash
git clone https://github.com/terroo/terlang
cd terlang\
cmake -B build .
cmake --build build

# Create destination folders and subfolders
New-Item -Path "C:\Program Files\Terlang\bin" -ItemType Directory -Force

# Move to destination folder
Move-Item -Path "build\Debug\ter.exe" -Destination "C:\Program Files\Terlang\bin\ter.exe"

# Create an environment variable for system "Path"
[System.Environment]::SetEnvironmentVariable("Path", $env:Path + ";C:\Program Files\Terlang\bin", [System.EnvironmentVariableTarget]::Machine)
```

Close PowerShell, then reopen and run:
> To test the `ter` command.
```cpp
prompt> ter
ter> output(args())
Ter/Terlang VERSION: 0.0.1
ter> exit
```

---

### Only installing on Windows or Ubuntu

Windows:
```bash
Invoke-WebRequest -Uri "https://github.com/terroo/terlang/releases/download/0.0.1/terlang-windows-0.0.1.zip"
```
+ Unzip
+ Create folders and subfolders: `C:\Program Files\Terlang\bin`
+ Move the `.exe` to the `bin\` subfolder
+ Add the path as an environment variable to just the `ter` command in *PowerShell* or *CMD*

Ubuntu:
```bash
wget https://github.com/terroo/terlang/releases/download/0.0.1/terlang-ubuntu-24-04-0.0.1.zip
unzip terlang-ubuntu-24-04-0.0.1.zip
sudo mv ter /usr/local/bin
```

And test:
```cpp
$ ter
ter> output(args())
Ter/Terlang VERSION: 0.0.1
ter> exit
```

---

## Using
COMMING SOON

---

## Tutorials
From [video](COMMING SOON).

---

## Extensions and LICENSE
Syntax highlight for Vim:

Syntax highlight for Neovim:

Syntax highlight for VS Code:
> Comming soon!

---

[LICENSE GNU GPLv3](./LICENSE)
