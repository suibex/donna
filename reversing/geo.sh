#!/bin/bash

as test.s -o tete;
ld tete -o test;
./donna_debug test ./banks/arm64.bank;
#./build_all.sh && ./donna_debug test ./banks/arm64.bank;
