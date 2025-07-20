#!/bin/bash

g++ $1.cpp -std=c++17 -lboost_system -lboost_charconv -lrt -lpthread -g -O0 -o $1
#export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
