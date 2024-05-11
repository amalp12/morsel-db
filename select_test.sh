#!/bin/bash

# Define the file names
outputCSV="./outputlog_select_3.csv"

# Generate random numbers for rows and columns
rows=480000
int_size=4
str_size=256
row_size=0

mkdir Tables

./gen_table.sh

# SQL query

export ROOTDIR=$(pwd)
# Remove output CSV if it exists
if [ -f "$outputCSV" ]; then
    rm "$outputCSV"
fi

# Create CSV header
echo "NUM_OF_CORE,MORSEL_SIZE,cols,recSize,TimeTaken" >> "$outputCSV"


./compile_release.sh

for ((no_of_cores = 16; no_of_cores <= 32; no_of_cores += 2)); do
# for ((cols = 3; cols <= 20; cols += 1)); do   
    # ./compile_release.sh
    # for ((no_of_cores = 2; no_of_cores <= 48; no_of_cores += 2)); do
    for ((cols = 3; cols <= 20; cols += 1)); do
        file1="./Tables/Table_$cols.csv"
        export TABLE_NAME="Table_$cols"
        export NUM_OF_COLS_Table_$cols=$cols
        ceil_cols=$(( (cols + 1) / 2 ))
        floor_cols=$(( cols / 2 ))
        export row_size=$(( ceil_cols * 4 + floor_cols * 256 ))
        export QUERY="SELECT Table_$cols.Column_1_Table_$cols FROM Table_$cols WHERE Table_$cols.Column_1_Table_$cols>0;"
        export NUM_OF_RECS_Table_$cols=$rows
        export NUM_OF_CORES=$no_of_cores

        for ((morselSize = row_size*10; morselSize <= row_size*100000; morselSize += (100 * row_size))); do
        export MORSEL_SIZE_Table_$cols=$morselSize
        export MORSEL_SIZE_TEMP=$morselSize
            ./build-release/src/dbapp
            # ./build-release/src/dbapp
            echo "/-------------------------------------MORSEL $morselSize COMPLETE -------------------------------------------/"
        done
    done
done

echo "/---------------------------------------------------TEST COMPLETE -------------------------------------------/"
