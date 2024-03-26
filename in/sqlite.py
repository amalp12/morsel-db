import sqlite3
import csv
import time

# Step 1: Read CSV File
def read_csv(file_path):
    data = []
    with open(file_path, 'r', newline='', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            data.append(row)
    return data

# Step 2: Create SQLite Database
def create_database():
    conn = sqlite3.connect(':memory:')
    return conn

# Step 3: Create Table and Import Data
def create_table_and_import_data(conn, data):
    cursor = conn.cursor()

    # Assuming first row contains headers
    headers = data[0][0].split(';')
    # print(headers)
    cursor.execute(f"CREATE TABLE data ({', '.join(headers)})")

    # Insert data
    placeholders_for_headers = {', '.join(headers)}
    # print (placeholders_for_headers)
    for row in data[1:]:
        r = row[0].split(';')
        #placeholders = ', '.join(['?' for _ in range(len(r))])
        query = f"INSERT INTO data({', '.join(placeholders_for_headers)}) VALUES({r[0]}, \"{r[1]}\", {r[2]});"
        # q1=f"INSERT INTO data (ID, Name, Age) VALUES(1, \"teRikM\", 53);"
        # print(query)
        cursor.execute(query)

    conn.commit()

# Step 4: Run Query and Benchmark Time
def benchmark_query(conn, query):
    start_time = time.time()
    cursor = conn.cursor()
    cursor.execute(query)
    # result = cursor.fetchall()
    # conn.commit()
    execution_time = time.time() - start_time
    # print("Query result:", result)
    # print("Execution time:", execution_time, "seconds")
    # print in miliseconds
    print("Execution time:", execution_time*1000, "miliseconds")

# Main function
def main():
    # File path to CSV
    csv_file = './test_table_1.csv'

    # Step 1: Read CSV File
    data = read_csv(csv_file)

    # Step 2: Create SQLite Database
    conn = create_database()

    # Step 3: Create Table and Import Data
    create_table_and_import_data(conn, data)

    # Step 4: Run Query and Benchmark Time
    query = "SELECT * FROM data WHERE Age > 0"
    benchmark_query(conn, query)

if __name__ == "__main__":
    main()

