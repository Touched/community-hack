#!/usr/bin/env bash
set -e

sudo mkdir -p /opt/devkitpro
(cd /opt/devkitpro && wget -qO- "http://downloads.sourceforge.net/project/devkitpro/devkitARM/devkitARM_r45/devkitARM_r45-x86_64-linux.tar.bz2" | sudo tar -xvj)
