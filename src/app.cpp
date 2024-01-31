#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <pg_query.h>

int main(int argc, char **argv) {

  while (true) {
    try {
      std::cout << "Enter your query: ";
      std::string query;
      std::getline(std::cin, query);

      // Exitdloop if the user enters 'exit' or 'quit'
      if (query == "exit" || query == "quit") {
        std::cout << "Exiting...\n";
        break;
      }

      std::cout << "Query[] : \"" << query << "\"\n";
      hsql::SQLParserResult result;
      hsql::SQLParser::parse(query, &result);

      if (result.isValid() && result.size() > 0) {
        const hsql::SQLStatement *statement = result.getStatement(0);

        if (statement->isType(hsql::kStmtSelect)) {
          const auto *select =
              static_cast<const hsql::SelectStatement *>(statement);
          hsql::printStatementInfo(statement);
          /* ... */
        }
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