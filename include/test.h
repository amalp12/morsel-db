#pragma once
#include <cstring>
#include <list>
#include <string>
#include <threads.h>

struct Row {
  int id;
  char name[STRING_SIZE];
  int age;
};

void initMorsel(int core, std::string tableName, int start_index,
                int end_index);

int create_table_test();

void createTableMetaAndRows(std::string tableName);

void destructRelcat();

void loadPrevTable();

int insertRowsRandom(std::list<int> colTypeList, std::string tableName);

void initMorselRandom(int core, std::string tableName, int start_index,
                      int end_index);

int create_table_test_random();

std::string get_env_var(std::string const &key);

int create_table_test_random(std::string tableName);
