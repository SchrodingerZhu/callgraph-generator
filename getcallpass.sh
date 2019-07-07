#!/bin/bash

tmpfilename=(date +%N)

clang++ -emit-llvm -S $1 -o /tmp/$tmpfilename
opt -load ./libcallpass.so -dumpcalls /tmp/$tmpfilename  >/dev/null #2>$2
rm /tmp/$tmpfilename
