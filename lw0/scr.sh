#!/bin/bash

fname=$(basename -s .c "$1")

if gcc $1 -o $fname.exe; then
  shift 1
  ./$fname.exe $@

else
  echo "Not enough arguments" >&2;
fi
