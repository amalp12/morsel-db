#!/bin/bash

export ROOTDIR=`pwd`
NUMA_NODES=`numactl --hardware | grep available | awk '{print $2}'`

if [ $NUMA_NODES -eq 2 ]; then
    export NUMA="_numa"
fi
# Define array with integer values
int_values=(4000 10000 40000 80000 400000 800000)

DATE=`date +"%Y-%m-%d-%H-%M-%S"`
mv out/out$NUMA out/out${NUMA}_${DATE}.bak
# Traverse the array
for value in "${int_values[@]}"; do
    # Set environment variable TEST_TABLE_SIZE with current value
    export TEST_TABLE_SIZE="$value"
    
    # Generate Table of size $value
    cd in
    python3 ./gen_test_table.py
    cd ..
    wc -l in/test_table.csv
    # Run your program and pipe output to a file

    ./build-release/tests/morsel_test >> "out/out${NUMA}}.csv"

done

echo "NoOfCores,MorselSize,NoOfRecords,TimeTaken" | cat - out/out${NUMA}.csv > out/temp && mv out/temp out${NUMA}.csv