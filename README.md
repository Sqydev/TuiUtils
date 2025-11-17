# WARNING: PROJECT INDEV

This project is in hard development and is NOT usable.  

And This readme is just a copy paste from other project for now




<img align="left" style="width:260px" src="https://github.com/Sqydev/ESClib/blob/ReWrite/Logo.png" width="288px"></img>

**ESClib is library that intreduces tui for the C programing language.**

SMFHEREggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg
SMFHEREggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggg

---

![GitHub License](https://img.shields.io/github/license/Sqydev/ESClib)
![GitHub Downloads (all assets, all releases)](https://img.shields.io/github/downloads/Sqydev/ESClib/total)
![GitHub Release](https://img.shields.io/github/v/release/Sqydev/ESClib)
![GitHub repo size](https://img.shields.io/github/repo-size/Sqydev/ESClib)
![GitHub top language](https://img.shields.io/github/languages/top/Sqydev/ESClib)
![GitHub commits since latest release](https://img.shields.io/github/commits-since/Sqydev/ESClib/latest)


## Selling off

- Clear and logical function names
- NO external dependencies
- FREE and OPEN SOURCE
- Suport for Linux AND Windows(can be used with macos)
- Syntax and writing style highly inspired by **raylib**(Even readme)

## Instalation & Usage

### Linux

#### Instalation

<a href="https://repology.org/project/referencing/versions">
    <img src="https://repology.org/badge/vertical-allrepos/ESClib.svg?columns=2" alt="Packaging status" align="right">
</a>

If your distribution’s package manager is listed on the right, you can install **ESClib** directly by running:
```sh
sudo YOUR-PACKAGE-MANAGER INSTALL-ARG ESClib
```

If your distro isn’t listed, see the [Universal Instalation](https://github.com/Sqydev/ESClib/edit/ReWrite/README.md#universal) section below.

#### Usage

Include the library header in your `.c` file:
```c
#include <esclib.h>
```

When compiling your code with gcc, add this flag:
```sh
-lesclib
```

### Universal 

#### Instalation

To install **ESClib** manually, run:
```sh
git clone https://github.com/Sqydev/ESClib.git
```
Then simply copy the contents of the include/ and lib/YOUROSNAME/ directories into your project folder - for example, into your compiler’s global include/ directory to make the library available to all projects.

#### Usage

Include the library header in your `.c` file. Use the relative or absolute path if the header is not in a standard include directory:
```c
#include <path/to/esclib.h>
```

Compile your code by using your compiler's variant of flags bellow(flags bellow are for gcc):
```sh
-L/path/to/lib -lesclib
```
