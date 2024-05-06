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

# Generate data for Table A
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

# Generate data for Table B
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

# Add a comment at the end to denote the end of the script
# (This line is not necessary for the functionality of the script)
