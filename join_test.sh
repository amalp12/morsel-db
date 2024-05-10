#!/bin/bash

# Define file paths
file1="./in/Table_A.csv"
file2="./in/Table_B.csv"
outputCSV="./outputlog_join.csv"

if [ -f "$outputCSV" ]; then
    rm "$outputCSV"
fi

echo "NUM_OF_CORES,MORSEL_SIZE_Table_A,MORSEL_SIZE_Table_B,NUM_OF_COLS_Table_A,NUM_OF_COLS_Table_B,recSizeA,recSizeB,TimeTaken" >> "$outputCSV"

export QUERY="SELECT Table_A.Column_1_Table_A,Table_B.Column_1_Table_B,Table_A.Column_3_Table_A,Table_B.Column_3_Table_B FROM Table_A INNER JOIN Table_B ON Table_A.Column_3_Table_A=Table_B.Column_3_Table_B;"

if [ -f "$file1" ]; then
    rm "$file1"
fi

if [ -f "$file2" ]; then
    rm "$file2"
fi

# Define parameters for data generation
rowsA=1000
rowsB=500

export ROOTDIR="/home/ssl/morsel-db"

# Compile the program
./compile_release.sh

# Run the program with different configurations
for ((colA = 3; colA <= 20; colA+=1)); do
    for ((colB = 3; colB <= 20; colB+=1)); do
        for ((i = 1; i <= rowsA; i+=1)); do
            echo -n "$i" >> "$file1"
            echo -n ";" >> "$file1"
            for ((j = 2; j <= colA; j++)); do
                if (( j % 2 != 0 )); then
                    data=$(( RANDOM % 100 ))
                else
                    num=$(( RANDOM % 100 ))
                    data="string$num"
                fi
                echo -n "$data" >> "$file1"
                if (( j < colA )); then
                    echo -n ";" >> "$file1"
                fi
            done
            echo >> "$file1"
        done

        for ((i = 1; i <= rowsB; i+=1)); do
            echo -n "$i" >> "$file2"
            echo -n ";" >> "$file2"
            for ((j = 2; j <= colB; j+=1)); do
                if (( j % 2 != 0 )); then
                    data=$(( RANDOM % 100 ))
                else
                    num=$(( RANDOM % 100 ))
                    data="string$num"
                fi
                echo -n "$data" >> "$file2"
                if (( j < colB )); then
                    echo -n ";" >> "$file2"
                fi
            done
            echo >> "$file2"
        done

        export NUM_OF_COLS_Table_A=$colA
        export NUM_OF_COLS_Table_B=$colB
        export NUM_OF_RECS_Table_A=$rowsA
        export NUM_OF_RECS_Table_B=$rowsB
        
        ceil_cols1=$(( (colA + 1) / 2 ))
        ceil_cols2=$(( (colB + 1) / 2 ))
        floor_cols1=$(( colA / 2 ))
        floor_cols2=$(( colB / 2 ))
        row_size_A=$(( ceil_cols1 * 4 + floor_cols1 * 256 ))
        row_size_B=$(( ceil_cols2 * 4 + floor_cols2 * 256 ))

        export row_size_A=$row_size_A
        export row_size_B=$row_size_B

        for ((num_cores = 8; num_cores <= 16; num_cores += 2)); do
            export NUM_OF_CORES=$num_cores

            for ((morselSize1 = row_size_A; morselSize1 <= row_size_A * 20; morselSize1 += row_size_A)); do
                for ((morselSize2 = row_size_B; morselSize2 <= row_size_B * 20; morselSize2 += row_size_B)); do
                    export MORSEL_SIZE_Table_A=$morselSize1
                    export MORSEL_SIZE_Table_B=$morselSize2
                    export MORSEL_SIZE_temp_join_result=$((morselSize1 + morselSize2))
                    ./build-release/src/dbapp
                    echo "/---------------------------------------------------MORSEL SIZES $morselSize1 AND $morselSize2 COMPLETE -------------------------------------------/"
                done
            done
        done
    done
done

echo "/---------------------------------------------------TEST COMPLETE -------------------------------------------/"
