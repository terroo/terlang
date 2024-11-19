
# [Ter/Terlang](./README.md)
> Programming language for scripting with syntax similar to C++

![Ter/Terlang](./logo-ter-terlang.png) 

---

## Summary
1. [Building and Installing on UNIX-style systems](#building-on-unix-style-systems)
2. [Building and Installing on Windows](#building-and-installing-on-windows)
3. [Only installing on Windows](#only-installing-on-windows)
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
cmake --build build
sudo install -v build/ter /usr/local/bin/ter
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
```bash
git clone https://github.com/terroo/terlang
cd terlang
cmake --build build
```
Open `.sln` and compile with Visual Studio C++

+ Create folders and subfolders: `C:\Program Files\Terlang\bin`
+ Move the `Debug\ter.exe` to the `bin\` subfolder
+ Add the path as an environment variable to just the `ter` command in *PowerShell* or *CMD*

---

### Only installing on Windows
```bash
Invoke-WebRequest -Uri "https://github.com/terroo/terlang/releases/download/0.0.1/terlang-win.zip"
```
+ Unzip
+ Create folders and subfolders: `C:\Program Files\Terlang\bin`
+ Move the `.exe` to the `bin\` subfolder
+ Add the path as an environment variable to just the `ter` command in *PowerShell* or *CMD*

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

[LICENSE]()
