#pragma once
#include <threads.h>
#include "morsel.h"
#include <cstring>

struct Row {
    int id;
    char name[STRING_SIZE];
    int age;
};


void initMorsel(int core , std::string tableName);

int create_table_test();