#!/bin/bash
c++ debug.cc -fPIC -shared -I../bank/ -std=c++11  -o donna_backend.so; 
