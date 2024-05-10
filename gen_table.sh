#!/bin/bash

rows=48000


for ((col = 3; col <= 20; col++)); do
    file1="./Tables/Table_$col.csv"
    if [ -f "$file1" ]; then
     continue
    fi
    for ((i = 1; i <= rows; i+=1)); do
            echo -n "$i" >> "$file1"
            echo -n ";" >> "$file1"
            for ((j = 2; j <= col; j++)); do
                if (( j % 2 != 0 )); then
                    data=$(( RANDOM % 100 ))
                else
                    num=$(( RANDOM % 100 ))
                    data="string$num"
                fi
                echo -n "$data" >> "$file1"
                if (( j < col )); then
                    echo -n ";" >> "$file1"
                fi
            done
            echo >> "$file1"
        done
    echo "/--------------------------$col COLS TABLE DONE--------------------------/"
done

