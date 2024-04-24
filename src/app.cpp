#include "dispatcher.h"
#include "lib.h"
#include "test.h"
#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <thread>

int main(int argc, char **argv) {

  int coreNum = 1;
  // create staticVars
  StaticVars staticVars;
  // set cores and morsel size
  staticVars.setNumberOfCores(4);

  staticVars.setMaxMorselSize(264000);
  // create_table_test();
  while (true) {
    // create_table_test();
    std::string table1 = "Table_A";
    std::string table2 = "Table_B";
    create_table_test_random(table1);
    create_table_test_random(table2);
    try {

      std::cout << "Enter your query: " << std::endl;

      // flush
      std::cout.flush();
      std::cin.clear();

      // SELECT Name, ID, Age from test_table WHERE Age > 0;
      std::string query;
      std::getline(std::cin, query);

      // Exit loop if the user enters 'exit' or 'quit'
      if (query == "exit" || query == "quit") {
        std::cout << "Exiting...\n";
        break;
      }

      std::cout << "Query[] : \"" << query << "\"\n";
      hsql::SQLParserResult result;
      hsql::SQLParser::parse(query, &result);

      if (result.isValid() && result.size() > 0) {
        const hsql::SQLStatement *statement = result.getStatement(0);

        QEP qep(statement);

        // call execute on the qep object
        // qep.execute(0);
        // number of cores
        int numberOfCores = staticVars.getNumberOfCores();

        if (isStatementMultithread(statement->type())) {
          //  create_table_test();
          // create 4 threads and call exicute on each
          //  std::list<std::thread> threads;
          std::thread threads[numberOfCores];

          // Launch threads
          for (int i = 0; i < numberOfCores; i++) {
            threads[i] = std::thread(qep.execute, i + 1);
          }

          // Join threads
          for (int i = 0; i < numberOfCores; i++) {
            threads[i].join();
          }

        }

        else {
          qep.execute(coreNum);
        }

        coreNum = (coreNum + 1) % numberOfCores;
      }

    } catch (...) {
      // Handle any exceptions if needed
      std::cerr << "An error occurred.\n";
    }
    destructRelcat();
  }

  return 0;
}