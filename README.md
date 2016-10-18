# Community Hack

## Building

Requirements:
- DevkitARM
- Python >3.4
- PyYAML
- Doyxgen
- Armips

Make sure your `DEVKITARM` environment variable. Windows users should use the MingGW shell that comes with DevkitARM to run the console commands. Make sure the `DEVKITARM` environment variable is set to a MinGW like `/c/devkitPRO/devkitARM/` not `C:\devkitPRO\devkitARM\`.

First, get the repository
```
git clone --recursive <url>
```

We use the tools from `pokeruby`, so make sure those are present:
```
cd deps/pokeruby
./build_tools.sh
cd -
```

Windows users will want to copy the contents of the `tools` directory from [here](https://github.com/YamaArashi/pokeruby-tools) to `deps/pokeruby/tools`.

Next, build the headers:
```
cd deps/pokeagb
make
cd -
```

Next, create a directory called `roms` and place a clean FireRed US version 1.0 ROM inside called `BPRE0.gba`.

Now we can build the project by running `make`.

If the last line is `Done.` then everything succeeded.

## Project Structure

The source is organised primarily in the following structure:

`patches/` - precise byte edits for existing code in the ROM
`images/` - all the images files that will be inserted into free space
`src/` -  new C source as well as anything that needs to be inserted into free space, such as scripts.
`src/hooks` - glue ASM code that is branched to by code in patches

These directories also contain sub-directories that allow the code to be grouped in specific clusters. For example, `src/mega`, `src/hooks/mega`, `patches/mega` contain all code related to Mega Evolution and Primal Reversion. There should also be a README for every subdirectory located in the `src` directory (e.g. `src/mega/README.md` for the README for Mega Evolution).

The files in `images` automatically are translated in C source and header files by `grit` during the build process. The output for these files is placed at `src/generated/images/` so one can easily include the relevant image files by doing
```c
#include "generated/images/relative/path/to/image.h"
```
in your source file.
