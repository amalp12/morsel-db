#include "test.h"
#include "relcat.h"
#include "static.h"
#include "tuple.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

// destructMorsel
void destructRelcat() {
  // create recat object

  RelationCatalog relCat;
  relCat.deleteRelcat();
}
std::string get_env_var(std::string const &key) {
  char *val;
  val = std::getenv(key.c_str());
  std::string retval = "";
  if (val != NULL) {
    retval = val;
  }
  return retval;
}
void initMorsel(int core, std::string tableName, int start_index,
                int end_index) {
  RelationCatalog relCat;
  Row *destination = new Row();
  std::string codebase = "/home/ssl/Code/db";
  std::ifstream file(codebase + "/in/" + tableName + ".csv");

  if (!file.is_open()) {
    std::cerr << "Failed to open file!" << std::endl;
    return;
  }

  int currentLine = 0;

  std::string line;
  while (std::getline(file, line)) {
    currentLine++;

    // Check if the current line is within the desired range
    if (currentLine < start_index)
      continue; // Skip lines until startLine is reached

    // Stop loading if we reach endLine
    if (currentLine > end_index)
      break;

    size_t pos1 = line.find(';');
    size_t pos2 = line.find(';', pos1 + 1);

    std::string idStr = line.substr(0, pos1);
    std::string nameStr = line.substr(pos1 + 1, pos2 - pos1 - 1);
    std::string ageStr = line.substr(pos2 + 1);

    int id = std::stoi(idStr);
    int age = std::stoi(ageStr);

    char name[STRING_SIZE];
    std::strcpy(name, nameStr.c_str());

    Row record;

    record.id = id;
    std::strcpy(record.name, name);
    record.age = age;

    memcpy(destination, &record, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName, core, destination);
  }

  delete destination;
}

int create_table_test(std::string tableName) {
  // staticvars
  StaticVars staticVars;

  RelationCatalog relCat;
  // SELECT Name FROM test_table WHERE Age > 25;
  // std::string tableName = "test_table";
  std::list<std::string> colNameList = {"ID", "Name", "Age"};
  std::list<int> colTypeList = {INTEGER, STRING, INTEGER};
  relCat.insertNewTable(tableName, colNameList, colTypeList);

  // initMorsel(0 , tableName);
  // Create threads
  std::vector<std::thread> threads;
  std::string filename = "/home/ssl/Code/db/in/" + tableName + ".csv";

  std::ifstream file(filename);
  int total_lines = std::count(std::istreambuf_iterator<char>(file),
                               std::istreambuf_iterator<char>(), '\n');
  int lines_per_thread = total_lines / staticVars.getNumberOfCores();

  // Launch threads
  for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
    int start_line = lines_per_thread * i + 1;

    int end_line = start_line + lines_per_thread - 1;
    threads.emplace_back(initMorsel, i + 1, tableName, start_line, end_line);
    // threads[i] = std::thread(initMorsel , i , tableName , i*lines_per_thread+
    // 1, (i+1)*10000);
  }

  // Join threads
  for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
    threads[i].join();
  }
  return 0;
}

int create_table_test_random(std::string tableName) {
  /** SELECT Table_A.Column_1_Table_A , Table_B.Column_1_Table_B
   * ,Table_A.Column_2_Table_A , Table_B.Column_2_Table_B FROM Table_A INNER
   * JOINTable_B ON Table_A.Column_2_Table_A = Table_B.Column_2_Table_B;*/
  StaticVars staticVars;
  std::list<std::string> colNameList;
  std::list<int> colTypeList;

  // generate the number of columns
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(3, 7);
  const int numColumns = dist(gen);

  RelationCatalog relCat;
  // print numColumns
  std::cout << tableName << " : " << numColumns << std::endl;

  for (int col = 0; col < numColumns; col++) {
    colNameList.push_back("Column_" + std::to_string(col + 1) + "_" +
                          tableName);
    colTypeList.push_back(
        static_cast<ColumnType>(col % 2)); // Cycling through column types
  }

  // store meta data of table generated
  std::string tableMetaDataFile =
      "/home/ssl/Code/db/in/test_table_meta_data.txt";
  std::ofstream outFile(tableMetaDataFile, std::ios_base::out);
  outFile << "Table Name:" << tableName << "\n";

  auto colNameIter = colNameList.begin();
  auto colTypeIter = colTypeList.begin();
  for (; colNameIter != colNameList.end() && colTypeIter != colTypeList.end();
       colNameIter++, colTypeIter++) {
    outFile << *colNameIter << ":";
    outFile << *colTypeIter << "\n";
  }

  relCat.insertNewTable(tableName, colNameList, colTypeList);
  int num_records = insertRowsRandom(colTypeList, tableName);

  RelationCatalogEntry *entry = RelationCatalog::getTableEntryRef(tableName);
  entry->num_records = num_records;

  // initMorsel(0 , tableName);
  // Create threads
  std::vector<std::thread> threads;
  std::string filename = "/home/ssl/Code/db/in/" + tableName + ".csv";
  std::ifstream file(filename);
  auto endBuff = std::istreambuf_iterator<char>();
  auto fileBuff = std::istreambuf_iterator<char>(file);
  int total_lines = std::count(fileBuff, endBuff, '\n');
  int lines_per_thread = total_lines / staticVars.getNumberOfCores();

  // Launch threads
  for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
    int start_line = lines_per_thread * i + 1;

    int end_line = start_line + lines_per_thread - 1;
    threads.emplace_back(initMorselRandom, i + 1, tableName, start_line,
                         end_line);
    // threads[i] = std::thread(initMorsel , i , tableName , i*lines_per_thread+
    // 1, (i+1)*10000);
  }

  // Join threads
  for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
    threads[i].join();
  }

  // print the table in a tuple stream
  // for each code
  for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
    auto tupleStream = new ReadTupleStream(entry, i + 1);
    tupleStream->writeStream("./test" + tableName + ".txt");
    delete tupleStream;
  }
  return 0;
}

void initMorselRandom(int core, std::string tableName, int start_index,
                      int end_index) {
  RelationCatalogEntry *entry = RelationCatalog::getTableEntryRef(tableName);
  std::list<Attribute> attributeList = entry->getAttributes();

  int entrySize = 0;
  for (auto iter = attributeList.begin(); iter != attributeList.end(); iter++) {
    entrySize += iter->size;
  }

  char *destination = new char[entrySize];
  int integerAttr;
  char stringAttr[STRING_SIZE] = {'\0'};

  std::string filename = "/home/ssl/Code/db/in/" + tableName + ".csv";
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cerr << "Failed to open file!" << std::endl;
    return;
  }

  int currentLine = 1;

  std::string line;

  RelationCatalog relCat;

  while (std::getline(file, line)) {

    // Check if the current line is within the desired range
    if (currentLine < start_index)
      continue; // Skip lines until startLine is reached

    // Stop loading if we reach endLine
    if (currentLine > end_index)
      break;

    std::stringstream ss(line);
    std::string cell;
    for (const auto &column : attributeList) {
      std::getline(ss, cell, ';');

      // check type of column
      if (column.type == STRING) {
        memcpy(destination + column.offset, cell.c_str(), cell.size() + 1);
      }

      if (column.type == INTEGER) {
        integerAttr = std::stoi(cell);
        memcpy(destination + column.offset, &integerAttr, column.size);
      }
    }

    relCat.appendToThreadMapMorsel(tableName, core, destination);
    currentLine++;
  }

  delete[] destination;
}

int insertRowsRandom(std::list<int> colTypeList, std::string tableName) {

  std::string filename = "/home/ssl/Code/db/in/" + tableName + ".csv";
  std::ofstream outFile(filename, std::ios_base::out);
  std::random_device rd;
  std::mt19937 gen(rd());
  //    std::uniform_int_distribution<int> distribution(10000, 100000);

  std::uniform_int_distribution<int> distribution(5000, 10000);
  const int num_rows = distribution(gen);

  //    int num_rows = distribution(gen);

  for (int i = 0; i < num_rows; i++) {
    int col = 0;
    for (const auto &column : colTypeList) {
      if (column == INTEGER) {
        std::uniform_int_distribution<int> distribution_col(1, 50);
        int num = distribution_col(gen);
        if (col != 0)
          outFile << ";" << num;
        else
          outFile << i + 1;
      }

      else if (column == STRING) {
        std::uniform_int_distribution<int> distribution_col(1, 50);
        int num = distribution_col(gen);
        if (col != 0)
          outFile << ";"
                  << "str" + std::to_string(num);
        else
          outFile << "str" + std::to_string(i + 1);
      }

      col++;
    }

    outFile << "\n";
  }

  return num_rows;
}