#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <pg_query.h>
#include "dispatcher.h"
#include <thread>
#include "test.h"
#include "lib.h"


int main(int argc, char **argv) {

  int coreNum = 1;
  // create staticVars
  StaticVars staticVars;
  // create_table_test();
  while (true) {
    create_table_test();
    try {
      std::cout << "Enter your query: ";
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

        // print the enum constant
        // std::cout << "Statement Greater: " << hsql::kOpGreater << std::endl;
        parseSelectStatement(statement);
        // test code
        // create a local qep object
       
        QEP qep(statement);

        // call execute on the qep object
        // qep.execute(0);

        if(isStatementMultithread(statement->type()))
        {
        //  create_table_test();
         // create 4 threads and call exicute on each
        //  std::list<std::thread> threads;
         std::thread threads[staticVars.getNumberOfCores()];

         //Launch threads
         for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
            threads[i] = std::thread(qep.execute, i+1);
            // threads.emplace_back(qep.execute, i);
         }

         // Join threads
         for (int i = 0; i < staticVars.getNumberOfCores(); i++) {
            threads[i].join();
         }

        }

        else
        {
           qep.execute(coreNum);
        }

        coreNum =  (coreNum + 1)%staticVars.getNumberOfCores();
        

        // if (statement->isType(hsql::kStmtSelect)) {
        //   const auto *select =
        //       static_cast<const hsql::SelectStatement *>(statement);
        //   hsql::printStatementInfo(statement);
        //   /* ... */
        // }
      }

      // PgQueryParseResult result2;
      // result2 = pg_query_parse(query.data());
      // std::cout << result2.parse_tree << "\n";
      // pg_query_free_parse_result(result2);
    } catch (...) {
      // Handle any exceptions if needed
      std::cerr << "An error occurred.\n";
    }
   destructRelcat();
  }

  return 0;
}