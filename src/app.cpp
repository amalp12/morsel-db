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

void initializeJoinHash(const hsql::SelectStatement *selectStatement,
                        std::string table1, std::string table2) {

  // if always linear search return
  if (ALWAYS_LINEAR_SEARCH) {
    return;
  }
  // get relcat

  // get table1
  // probe table is the smaller table and build table is the larger table

  // get the table entry for table1
  RelationCatalogEntry *probeTableEntry =
      RelationCatalog::getTableEntryRef(table1);
  // get the table entry for table2
  RelationCatalogEntry *buildTableEntry =
      RelationCatalog::getTableEntryRef(table2);

  std::string joinStatementAttributeLeft =
      selectStatement->fromTable->join->condition->expr->name;
  std::string joinStatementAttributeRight =
      selectStatement->fromTable->join->condition->expr2->name;
  if (probeTableEntry->num_records > buildTableEntry->num_records) {
    std::swap(probeTableEntry, buildTableEntry);
    std::swap(joinStatementAttributeLeft, joinStatementAttributeRight);
  }

  // get the name of the attributes in the build table that is supposed to be
  // indexed usig b+ tree

  // std::vector<std::string> buildTableIndexAttributes;
  // get the list of column names of the build table that is used in the join
  // statement

  // copy attr list of builtable to a list
  std::list<Attribute> *buildTableAttrList =
      buildTableEntry->getAttributesRef();
  for (auto &attr : *buildTableAttrList) {
    // get attributes in the join statement

    if (attr.name == joinStatementAttributeRight) {
      // buildTableIndexAttributes.push_back(attr.name);
      // create index on the attribute
      if (!attr.isIndexed) {
        attr.isIndexed = true;
        // create index on the attribute
        attr.bPlusTreeContainer = new BPlusTreeContainer(attr.name);
        break;
      }
    }
  }
}

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

        if (statement->type() == hsql::kStmtSelect) {

          // typecast to select statement
          const hsql::SelectStatement *selectStatement =
              (const hsql::SelectStatement *)statement;
          // check if join
          if (selectStatement->fromTable->type == hsql::kTableJoin) {
            // initialize join hash
            initializeJoinHash(selectStatement, table1, table2);
          }
        }
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

// SELECT Table_A.Column_1_Table_A FROM Table_A WHERE
// Table_A.Column_1_Table_A>0;
