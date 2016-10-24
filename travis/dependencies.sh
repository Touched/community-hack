#!/usr/bin/env bash
set -e

# Download and configure devkitARM
sudo mkdir -p /opt/devkitpro
(cd /opt/devkitpro && wget -qO- "http://downloads.sourceforge.net/project/devkitpro/devkitARM/devkitARM_r45/devkitARM_r45-x86_64-linux.tar.bz2" | tar -xvj)

export DEVKITPRO=/opt/devkitpro
export DEVKITARM=$DEVKITPRO/devkitARM
export PATH=$PATH:$DEVKITARM/bin

# Configure other dependencies
(cd deps/pokeagb && make)
(cd deps/pokeruby && ./build_tools.sh)
(cd deps/patch && make)
