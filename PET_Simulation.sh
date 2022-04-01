#!/bin/bash
clear
echo "Compiling main.cpp to main.out..."

# Compiling simulation.cpp with -Ofast
g++ "src/simulation/simulation.cpp" -Wno-format-truncation  -pedantic -std=c++11 -lpthread -ldl -o "bin/simulation.out" -Ofast
echo simulation.cpp compiled, executing main.out

# Executing simulation.out and grepping OUTPUT line
OUTPUT=$(./bin/simulation.out | grep "OUTPUT")

# Splitting output into an array
declare -a ARGS
declare -i j=0
for i in ${OUTPUT[@]}
    do
        ARGS[j]=$i;
        j=$((j+1));
    done
echo "Simulation Ended!"
echo "Starting analysis:"

# Calling analysis by passing output arguments
root "src/analysis/analysis.cpp(${ARGS[1]},${ARGS[2]})"

# Opening TBroser on saved analysis file
(cd data/analysis; root -l "../../src/analysis/browser.cpp")

# Asking for user keypress
read -n 1 -s -r -p "Press any key to continue..."
clear