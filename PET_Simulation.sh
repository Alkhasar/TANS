#!/bin/bash
clear
echo "Compiling main.cpp to main.out..."
#root main.cpp
g++ main.cpp -Wno-format-truncation -Ofast -pedantic -std=c++11 -lpthread -ldl -o main.out
echo main.cpp compiled, executing main.out
./main.out
echo "Simulation Ended!"
echo "Starting analysis:"
root analysis.cpp
read -n 1 -s -r -p "Press any key to continue..."
clear