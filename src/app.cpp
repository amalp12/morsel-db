#include "dispatcher.h"
#include "lib.h"
#include "relcat.h"
#include "static.h"
#include "test.h"
#include <hsql/SQLParser.h>
#include <hsql/sql/SQLStatement.h>
#include <hsql/sql/Table.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <iomanip>
#include <numa.h>

void initializeJoinHash(const hsql::SelectStatement *selectStatement,
                        std::string table1, std::string table2) {
    if (ALWAYS_LINEAR_SEARCH) {
        return;
    }

    RelationCatalogEntry *probeTableEntry = RelationCatalog::getTableEntryRef(table1);
    RelationCatalogEntry *buildTableEntry = RelationCatalog::getTableEntryRef(table2);

    std::string joinStatementAttributeLeft = selectStatement->fromTable->join->condition->expr->name;
    std::string joinStatementAttributeRight = selectStatement->fromTable->join->condition->expr2->name;

    if (probeTableEntry->num_records > buildTableEntry->num_records) {
        std::swap(probeTableEntry, buildTableEntry);
        std::swap(joinStatementAttributeLeft, joinStatementAttributeRight);
    }

    std::list<Attribute> *buildTableAttrList = buildTableEntry->getAttributesRef();
    for (auto &attr : *buildTableAttrList) {
        if (attr.name == joinStatementAttributeRight && !attr.isIndexed) {
            attr.isIndexed = true;
            attr.bPlusTreeContainer = new BPlusTreeContainer(attr.name);
            break;
        }
    }
}

int main(int argc, char **argv) {
    StaticVars staticVars;
    
    // initialize numa
    if (numa_available() < 0 ) {
        std:: cout << "Numa not available!";
        exit(0);
    }
    staticVars.setNumaNodes(numa_max_node()+1);
    // staticVars.setMaxMorselSize(std::stoi(get_env_var("MORSEL_SIZE")));
    staticVars.setNumberOfCores(std::stoi(get_env_var("NUM_OF_CORES")));

  int coreNum = 1;
  // create staticVars
  // StaticVars staticVars;
  // set cores and morsel size
  // staticVars.setNumberOfCores(std::stoi(get_env_var("NUM_OF_CORES")));
  // staticVars.setNumberOfCores(4);


    while (true) {
        std::string table = "test_table";
        std::string table1 = "Table_A";
        std::string table2 = "Table_B";
        create_table_test_random(table);

        try {
            const std::string outputCSV = "./outputlog_select.csv";
            std::ofstream output(outputCSV.c_str(), std::ios_base::app);

            std::string query = get_env_var("QUERY");
            if (query == "exit" || query == "quit") {
                std::cout << "Exiting...\n";
                break;
            }

            hsql::SQLParserResult result;
            hsql::SQLParser::parse(query, &result);

            if (result.isValid() && result.size() > 0) {
                const hsql::SQLStatement *statement = result.getStatement(0);
                QEP qep(statement);

                if (statement->type() == hsql::kStmtSelect) {
                    
                    const hsql::SelectStatement * selectStatement = (const hsql::SelectStatement *)statement;

                    if(selectStatement->fromTable->type == hsql::kTableJoin)
                    {
                      initializeJoinHash((const hsql::SelectStatement *)statement, table1, table2);
                    }
                }

                float res;
                if (isStatementMultithread(statement->type())) {
                    std::vector<int> timeArr(staticVars.getNumberOfCores());

                    std::vector<std::thread> threads(staticVars.getNumberOfCores());
                    for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
                        threads[t_no] = std::thread([&timeArr, &qep, t_no]() {
                            timeArr[t_no] = qep.execute(t_no + 1);
                        });
                    }

                    res = 0;
                    for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
                        threads[t_no].join();
                        res += timeArr[t_no];
                    }
                    res /= staticVars.getNumberOfCores();
                } else {
                    res = qep.execute(1); // Assuming coreNum is not used elsewhere
                }

                int cols = std::stoi(get_env_var("NUM_OF_COLS_" + table));
                
                

                output << staticVars.getNumberOfCores() 
                << "," << std::stoi(get_env_var("MORSEL_SIZE_" + table)) 
                << "," << std::stoi(get_env_var("NUM_OF_COLS_" + table)) 
                << "," << std::stoi(get_env_var("row_size"))
                << "," << std::fixed << std::setprecision(5) << res << std::endl;
                }

        } catch (...) {
            std::cerr << "An error occurred.\n";
        }
        destructRelcat();
        break;
    }

    return 0;
}


// #include "dispatcher.h"
// #include "lib.h"
// #include "relcat.h"
// #include "static.h"
// #include "test.h"
// #include <hsql/SQLParser.h>
// #include <hsql/sql/SQLStatement.h>
// #include <hsql/sql/Table.h>
// #include <hsql/util/sqlhelper.h>
// #include <iostream>
// #include <string>
// #include <thread>
// #include <fstream>

// void initializeJoinHash(const hsql::SelectStatement *selectStatement,
//                         std::string table1, std::string table2) {

//   // if always linear search return
//   if (ALWAYS_LINEAR_SEARCH) {
//     return;
//   }
//   // get relcat

// //   // get table1
// //   // probe table is the smaller table and build table is the larger table

//   // get the table entry for table1
//   RelationCatalogEntry *probeTableEntry =
//       RelationCatalog::getTableEntryRef(table1);
//   // get the table entry for table2
//   RelationCatalogEntry *buildTableEntry =
//       RelationCatalog::getTableEntryRef(table2);

//   std::string joinStatementAttributeLeft =
//       selectStatement->fromTable->join->condition->expr->name;
//   std::string joinStatementAttributeRight =
//       selectStatement->fromTable->join->condition->expr2->name;
//   if (probeTableEntry->num_records > buildTableEntry->num_records) {
//     std::swap(probeTableEntry, buildTableEntry);
//     std::swap(joinStatementAttributeLeft, joinStatementAttributeRight);
//   }

// //   // get the name of the attributes in the build table that is supposed to be
// //   // indexed usig b+ tree

//   // std::vector<std::string> buildTableIndexAttributes;
//   // get the list of column names of the build table that is used in the join
//   // statement

//   // copy attr list of builtable to a list
//   std::list<Attribute> *buildTableAttrList =
//       buildTableEntry->getAttributesRef();
//   for (auto &attr : *buildTableAttrList) {
//     // get attributes in the join statement

//     if (attr.name == joinStatementAttributeRight) {
//       // buildTableIndexAttributes.push_back(attr.name);
//       // create index on the attribute
//       if (!attr.isIndexed) {
//         attr.isIndexed = true;
//         // create index on the attribute
//         attr.bPlusTreeContainer = new BPlusTreeContainer(attr.name);
//         break;
//       }
//     }
//   }
// }

// int main(int argc, char **argv) {

//   int coreNum = 1;
//   // create staticVars
//   StaticVars staticVars;
//   // set cores and morsel size
//   // staticVars.setNumberOfCores(4);
//   staticVars.setMaxMorselSize(std::stoi(get_env_var("MORSEL_SIZE")));
//   staticVars.setNumberOfCores(std::stoi(get_env_var("NUM_OF_CORES")));
//   // staticVars.setMaxMorselSize(10000);
//   // create_table_test();
//   while (true) {
//     // create_table_test();
//     std::string table1 = "test_table";
//     // std::string table2 = "Table_B";
//     create_table_test_random(table1);
//     // create_table_test_random(table2);
//     try {

//       std::cout << "Enter your query: " << std::endl;

//       // flush
//       std::cout.flush();
//       std::cin.clear();

//       const std::string outputCSV = "./outputlog_100000recs.csv";

//       std::ofstream output(outputCSV.c_str(), std::ios_base::app);

//       // output << "NUM_OF_CORE,MORSEL_SIZE,cols,recSize,TimeTaken" << std::endl;

//       // SELECT Name, ID, Age from test_table WHERE Age > 0;
//       int numberOfCores = staticVars.getNumberOfCores();
//       std::string query;
//       float res;
//       std::array<int, 48> timeArr;
//       // std::getline(std::cin, query);
//       query = get_env_var("QUERY");
//       // Exit loop if the user enters 'exit' or 'quit'
//       if (query == "exit" || query == "quit") {
//         std::cout << "Exiting...\n";
//         break;
//       }

//       std::cout << "Query[] : \"" << query << "\"\n";
//       hsql::SQLParserResult result;
//       hsql::SQLParser::parse(query, &result);

//       if (result.isValid() && result.size() > 0) {
//         const hsql::SQLStatement *statement = result.getStatement(0);

//         QEP qep(statement);

//         if (statement->type() == hsql::kStmtSelect) {

//           // typecast to select statement
//           const hsql::SelectStatement *selectStatement =
//               (const hsql::SelectStatement *)statement;
//           // check if join
//           if (selectStatement->fromTable->type == hsql::kTableJoin) {
//             // initialize join hash
//             // initializeJoinHash(selectStatement, table1, table2);
//           }
//         }
//         // call execute on the qep object
//         // qep.execute(0);
//         // number of cores

//         if (isStatementMultithread(statement->type())) {
//           //  create_table_test();
//           // create 4 threads and call exicute on each
//           //  std::list<std::thread> threads;
//           std::vector<std::thread> threads(staticVars.getNumberOfCores());

//             // Launch threads
//             for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
//               threads[t_no] = std::thread(
//                   [&timeArr, &qep,
//                    t_no]() { // Capture timeArr in the lambda capture list
//                     timeArr[t_no] = qep.execute(t_no + 1);
//                   });
//             }
//             // reset res
//             res = 0;
//             // Join threads
//             for (int t_no = 0; t_no < staticVars.getNumberOfCores(); t_no++) {
//               threads[t_no].join();
//               res += timeArr[t_no];
//             }
//             // divide res by the number of cores
//             res /= staticVars.getNumberOfCores();
//         }

//         else {
//           res = qep.execute(coreNum);
//         }
        
//         int cols = std::stoi(get_env_var("NUM_OF_COLS_test_table"));
//         int ceil_cols = (cols + 1) / 2;
//         int floor_cols = cols / 2;
//         int row_size = ceil_cols * 4 + floor_cols * 256;

//         coreNum = (coreNum + 1) % numberOfCores;
//         output <<  staticVars.getNumberOfCores() << "," <<  staticVars.getMaxMorselSize() << "," << cols << "," <<  row_size << ","  << res << std::endl;
//       }

//     } catch (...) {
//       // Handle any exceptions if needed
//       std::cerr << "An error occurred.\n";
//     }
//     destructRelcat();
//     break;
//   }

//   return 0;
// }