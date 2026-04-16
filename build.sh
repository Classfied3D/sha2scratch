#!/bin/sh

SOURCE_DIR="${PWD}"
cd ../..
CFLAGS="-I \"$SOURCE_DIR\"" ./build.sh -O0 "$SOURCE_DIR/main.c" "$SOURCE_DIR/sha-256.c"
