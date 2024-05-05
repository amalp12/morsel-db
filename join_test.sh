#!/bin/bash

# Define the file names
file1="./in/Table_A.csv"
file2="./in/Table_B.csv"
outputCSV="./outputlog_join.csv"

# Generate random numbers for rows and columns
rowsA=100000
rowsB=50000
int_size=4
str_size=256
row_size_A=0
row_size_B=0

export NUM_OF_RECS_Table_A=$rowsA
export NUM_OF_RECS_Table_B=$rowsB

# SQL query
export QUERY="SELECT Table_A.Column_1_Table_A ,Table_B.Column_1_Table_B,Table_A.Column_3_Table_A , Table_B.Column_3_Table_B FROM Table_A INNER JOIN Table_B ON Table_A.Column_3_Table_A = Table_B.Column_3_Table_B;"
export ROOTDIR="/home/ssl/morsel-db"

# Remove output CSV if it exists
if [ -f "$outputCSV" ]; then
    rm "$outputCSV"
fi

# Create CSV header
echo "NUM_OF_CORE,MORSEL_SIZE_A,MORSEL_SIZE_B,colsA,colsB,recSizeA,recSizeB,TimeTaken" >> "$outputCSV"

for ((col1 = 3; col1 <= 10; col1 += 2)); do
    for ((col2 = 3; col2 <= 10; col2 += 2)); do
        rm "$file1" # Remove CSV file if it exists
        export NUM_OF_COLS_Table_A=$col1
        export NUM_OF_COLS_Table_B=$col2
        ceil_cols1=$(( (col1 + 1) / 2 ))
        ceil_cols2=$(( (col2 + 1) / 2 ))
        floor_cols1=$(( col1 / 2 ))
        floor_cols2=$(( col2 / 2 ))
        export row_size_A=$(( ceil_cols1 * 4 + floor_cols1 * 256 ))
        export row_size_B=$(( ceil_cols2 * 4 + floor_cols2 * 256 ))
        
        for ((i = 1; i <= rowsA; i++)); do
            echo -n "$i" >> "$file1"
            echo -n ";" >> "$file1"
            for ((j = 2; j <= col1; j++)); do
                if (( j % 2 != 0 )); then
                    data=$(( RANDOM % 100 ))
                else
                    num=$(( RANDOM % 100 ))
                    data="string$num"
                fi
                echo -n "$data" >> "$file1"
                if (( j < col1 )); then
                    echo -n ";" >> "$file1"
                fi
            done
            echo >> "$file1"
        done
        
        for ((j = 1; j <= rowsB; j++)); do
            echo -n "$j" >> "$file2"
            echo -n ";" >> "$file2"
            for ((k = 2; k <= col2; k++)); do
                if (( k % 2 != 0 )); then
                    data=$(( RANDOM % 100 ))
                else
                    num=$(( RANDOM % 100 ))
                    data="string$num"
                fi
                echo -n "$data" >> "$file2"
                if (( k < col2 )); then
                    echo -n ";" >> "$file2"
                fi
            done
            echo >> "$file2"
        done
        
        ./compile.sh
        for ((no_of_cores = 2; no_of_cores <= 48; no_of_cores += 2)); do
            export NUM_OF_CORES=$no_of_cores
            for ((morselSize1 = row_size_A; morselSize1 <= row_size_A * 8000; morselSize1 += row_size_A * 200)); do
                for ((morselSize2 = row_size_B; morselSize2 <= row_size_B * 8000; morselSize2 += row_size_B * 200)); do
                    export MORSEL_SIZE_Table_A=$morselSize1
                    export MORSEL_SIZE_Table_B=$morselSize2
                    export MORSEL_SIZE_temp_join_result=$((morselSize1 + morselSize2))
                    ./build/src/dbapp
                    echo "/---------------------------------------------------MORSEL SIZES  $morselSize1  AND  $morselSize2 COMPLETE -------------------------------------------/"

                done 
            done
            echo "/---------------------------------------------------CORE $no_of_cores COMPLETE -------------------------------------------/"
        done
    done
done

echo "/---------------------------------------------------TEST COMPLETE -------------------------------------------/"
