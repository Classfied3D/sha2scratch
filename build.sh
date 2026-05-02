#!/bin/sh

OPTLEVEL="z"

while getopts "O:" opt; do
  case $opt in
    O) OPTLEVEL="$OPTARG" ;;
  esac
done

SOURCE_DIR="${PWD}"
cd ../..
CFLAGS="-I \"$SOURCE_DIR\"" ./build.sh -O$OPTLEVEL "$SOURCE_DIR/main.c" "$SOURCE_DIR/sha-256.c"
