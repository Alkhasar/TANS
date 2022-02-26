#!/bin/bash
clear
echo "Compiling main.cpp to main.out..."
g++ main.cpp -Wno-format-truncation -Ofast -pedantic -std=c++11 -lpthread -ldl -o main.out
echo main.cpp compiled, executing main.out
./main.out
echo "Simulation Ended!"
read -n 1 -s -r -p "Press any key to continue..."
clear