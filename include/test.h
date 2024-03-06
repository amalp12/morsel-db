#pragma once
#include <threads.h>
#include "morsel.h"
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


struct Row {
    int id;
    char name[STRING_SIZE];
    int age;
};


void initMorsel(int core , std::string tableName , int start_index , int end_index);

int create_table_test();