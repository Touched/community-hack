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
