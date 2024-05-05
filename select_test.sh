#!/bin/bash

# Define the file names
file1="./in/test_table.csv"
outputCSV="./outputlog_100000recs.csv"

# Generate random numbers for rows and columns
rows=100000
int_size=4
str_size=256
row_size=0

export NUM_OF_RECS_test_table=$rows

# SQL query
export QUERY="SELECT test_table.Column_1_test_table FROM test_table WHERE test_table.Column_1_test_table>0;"
export ROOTDIR="/home/ssl/morsel-db"
# Remove output CSV if it exists
if [ -f "$outputCSV" ]; then
    rm "$outputCSV"
fi

# Create CSV header
echo "NUM_OF_CORE,MORSEL_SIZE,cols,recSize,TimeTaken" >> "$outputCSV"

for ((cols = 3; cols <= 20; cols += 2)); do
    rm "$file1" # Remove CSV file if it exists
    export NUM_OF_COLS_test_table=$cols
    ceil_cols=$(( (cols + 1) / 2 ))
    floor_cols=$(( cols / 2 ))
    export row_size=$(( ceil_cols * 4 + floor_cols * 256 ))
    
    for ((i = 1; i <= rows; i++)); do
        echo -n "$i" >> "$file1"
        echo -n ";" >> "$file1"
        for ((j = 2; j <= cols; j++)); do
            if (( j % 2 != 0 )); then
                data=$(( RANDOM % 100 ))
            else
                num=$(( RANDOM % 100 ))
                data="string$num"
            fi
            echo -n "$data" >> "$file1"
            if (( j < cols )); then
                echo -n ";" >> "$file1"
            fi
        done
        echo >> "$file1"
    done
    
    ./compile.sh
    for ((no_of_cores = 2; no_of_cores <= 48; no_of_cores += 2)); do
        export NUM_OF_CORES=$no_of_cores
        for ((morselSize = row_size; morselSize <= row_size * 8000; morselSize += row_size * 200)); do
            export MORSEL_SIZE_test_table=$morselSize
            export MORSEL_SIZE_test_table_temp=$morselSize
            ./build/src/dbapp
        done
        echo "/-------------------------------------CORE $no_of_cores COMPLETE -------------------------------------------/"
    done
done

echo "/---------------------------------------------------TEST COMPLETE -------------------------------------------/"
