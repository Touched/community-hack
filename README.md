# Community Hack

## Building
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

Now we can build the project by running `make`.
