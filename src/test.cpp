#include "test.h"


void initMorsel(int core , std::string tableName)
{
    RelationCatalog relCat;
    Row hardcodedRow = {1, "John", 30};
    void* destination = malloc(sizeof(Row));
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);

    hardcodedRow = {2, "Bill", 25};
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);

    hardcodedRow = {3, "Mary", 21};
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);

    hardcodedRow = {4, "Ken", 35};
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);

    hardcodedRow = {5, "Laura", 27};
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);

    hardcodedRow = {6, "Jack", 26};
    memcpy(destination, &hardcodedRow, sizeof(Row));
    relCat.appendToThreadMapMorsel(tableName , core , destination);
    
}



int create_table_test(){
    
    RelationCatalog relCat;
    // SELECT Name FROM test_table WHERE Age > 25;
    std::string tableName = "test_table";
    std::vector<std::string> colNameList = {"ID" , "Name" , "Age"};
    std::vector<int>  colTypeList = {INTEGER , STRING , INTEGER};
    relCat.insertNewTable(tableName , colNameList , colTypeList);
    
    initMorsel(0 , tableName);
    //Create threads
    std::thread threads[4];

    //Launch threads
    for (int i = 0; i < 4; ++i) {
        // threads[i] = std::thread(initMorsel , i , tableName);
    }

    // Join threads
    for (int i = 0; i < 4; ++i) {
        // threads[i].join();
    }
    return 0;
}