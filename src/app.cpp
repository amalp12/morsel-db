#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <pg_query.h>
#include "dispatcher.h"
#include <thread>
#include "test.h"
void parseSelectStatement(const hsql::SQLStatement* statement) {
    if (statement->type() == hsql::kStmtSelect) {
        const auto* selectStatement = static_cast<const hsql::SelectStatement*>(statement);
        // Parse columns
        std::cout << "Columns: ";
        for (const auto* column : *selectStatement->selectList) {
            if (column->type == hsql::kExprColumnRef) {
                std::cout << column->name << " ";
            }
        }
        std::cout << std::endl;

        if (selectStatement->fromTable != nullptr) {
            std::cout << "Table: " << selectStatement->fromTable->name << std::endl;
        }

        if (selectStatement->whereClause != nullptr) {
            std::cout << "WHERE: \n"; ;hsql::printExpression(selectStatement->whereClause, 1);
        }
    }
}

int main(int argc, char **argv) {


  create_table_test();
  while (true) {
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
        // parseSelectStatement(statement);
        // test code
        // create a local qep object
       
        QEP qep(statement);

        // call execute on the qep object
        qep.execute(0);
        
        // create 4 threads and call exicute on each
        // std::vector<std::thread> threads;
        // for (int i = 0; i < 4; i++) {
          // threads.push_back(std::thread(&QEP::execute, &qep, i));
          // threads[i].join();
        // }

        // if (statement->isType(hsql::kStmtSelect)) {
        //   const auto *select =
        //       static_cast<const hsql::SelectStatement *>(statement);
        //   hsql::printStatementInfo(statement);
        //   /* ... */
        // }
      }

      PgQueryParseResult result2;
      result2 = pg_query_parse(query.data());
      std::cout << result2.parse_tree << "\n";
      pg_query_free_parse_result(result2);
    } catch (...) {
      // Handle any exceptions if needed
      std::cerr << "An error occurred.\n";
    }
  }

  return 0;
}