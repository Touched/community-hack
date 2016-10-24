#!/usr/bin/env bash
set -e

(cd deps/pokeagb && make)
(cd deps/pokeruby && ./build_tools.sh)
(cd deps/patch && make)
