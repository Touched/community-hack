#!/usr/bin/env bash

set -e
PORT=2345

EMULATOR=mgba

# Start mGBA in the background and store it's PID
$EMULATOR --gdb $* &
EMULATOR_PID=$!

# Wait for mGBA to start the GDB server
while ! lsof -i :$PORT; do sleep 0.1; done

# Start GDB
(cd ${BASH_SOURCE%/*} && arm-none-eabi-gdb --quiet --command=init.gdb)

# Kill off the emulator when GDB exits
kill -9 $EMULATOR_PID
