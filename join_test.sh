#!/bin/bash

# Define file paths
file1="./in/Table_A.csv"
file2="./in/Table_B.csv"
outputCSV="./outputlog_join.csv"

# Define parameters for data generation
rowsA=100000
rowsB=50000
col1=3
col2=5
export NUM_OF_COLS_Table_A=$col1
export NUM_OF_COLS_Table_B=$col2
export NUM_OF_RECS_Table_A=$rowsA
export NUM_OF_RECS_Table_B=$rowsB
# Remove output CSV if it exists
if [ -f "$outputCSV" ]; then
    rm "$outputCSV"
fi

export QUERY="SELECT Table_A.Column_1_Table_A,Table_B.Column_1_Table_B,Table_A.Column_3_Table_A,Table_B.Column_3_Table_B FROM Table_A INNER JOIN Table_B ON Table_A.Column_3_Table_A=Table_B.Column_3_Table_B;"

# Create CSV header
echo "NUM_OF_CORE,MORSEL_SIZE_A,MORSEL_SIZE_B,colsA,colsB,recSizeA,recSizeB,TimeTaken" >> "$outputCSV"

# Calculate row sizes
ceil_cols1=$(( (col1 + 1) / 2 ))
ceil_cols2=$(( (col2 + 1) / 2 ))
floor_cols1=$(( col1 / 2 ))
floor_cols2=$(( col2 / 2 ))
export row_size_A=$(( ceil_cols1 * 4 + floor_cols1 * 256 ))
export row_size_B=$(( ceil_cols2 * 4 + floor_cols2 * 256 ))



# Compile the program
./compile_release.sh

# Run the program with different configurations
for ((no_of_cores = 8; no_of_cores <= 8; no_of_cores += 2)); do
    export NUM_OF_CORES=$no_of_cores
    for ((morselSize1 = row_size_A; morselSize1 <=  row_size_A; morselSize1 += row_size_A)); do
        for ((morselSize2 = row_size_B; morselSize2 <=  row_size_B; morselSize2 += row_size_B )); do
            export MORSEL_SIZE_Table_A=$morselSize1
            export MORSEL_SIZE_Table_B=$morselSize2
            export MORSEL_SIZE_temp_join_result=$((morselSize1 + morselSize2))
            ./build-release/src/dbapp
            echo "/---------------------------------------------------MORSEL SIZES  $morselSize1  AND  $morselSize2 COMPLETE -------------------------------------------/"
            break
        done
        break 
    done
    break
    echo "/---------------------------------------------------CORE $no_of_cores COMPLETE -------------------------------------------/"
    break
done

echo "/---------------------------------------------------TEST COMPLETE -------------------------------------------/"

