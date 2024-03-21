#include "test.h"

// destructMorsel
void destructRelcat(){
    // create recat object 

    RelationCatalog relCat;
    relCat.deleteRelcat();
    
}

void initMorsel(int core , std::string tableName , int start_index , int end_index)
{
    RelationCatalog relCat;
    Row* destination = new Row();
    std::ifstream file("/home/ssl/Code/db/in/test_table.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return;
    }

    int currentLine = 0;

    std::string line;
    while (std::getline(file, line))
    {
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
        relCat.appendToThreadMapMorsel(tableName , core , destination);

       
    }

    delete destination;
    
}



int create_table_test(){
    // staticvars
    StaticVars staticVars;
    
    RelationCatalog relCat;
    // SELECT Name FROM test_table WHERE Age > 25;
    std::string tableName = "test_table";
    std::list<std::string> colNameList = {"ID" , "Name" , "Age"};
    std::list<int>  colTypeList = {INTEGER , STRING , INTEGER};
    relCat.insertNewTable(tableName , colNameList , colTypeList);
    
    // initMorsel(0 , tableName);
    //Create threads
    std::vector<std::thread> threads;
    const std::string filename("/home/ssl/Code/db/in/test_table.csv");
    std::ifstream file(filename);
    int total_lines = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');
    int lines_per_thread = total_lines / staticVars.getNumberOfCores();


    //Launch threads
    for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
        int start_line = lines_per_thread*i + 1;
       
        int end_line = start_line + lines_per_thread - 1;
        threads.emplace_back(initMorsel, i+1 ,tableName, start_line, end_line);
        // threads[i] = std::thread(initMorsel , i , tableName , i*lines_per_thread+ 1, (i+1)*10000);
    }

    // Join threads
    for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
        threads[i].join();
    }
    return 0;
}

