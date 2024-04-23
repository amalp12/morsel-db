#include "lib.h"
#include "constants.h"
#include <iostream>

bool compareInt(int value1, int value2, int operatorType) {
  switch (operatorType) {
  case EQUAL:
    return value1 == value2;
  case NOT_EQUAL:
    return value1 != value2;
  case LESS_THAN:
    return value1 < value2;
  case GREATER_THAN:
    return value1 > value2;
  case LESS_THAN_OR_EQUAL:
    return value1 <= value2;
  case GREATER_THAN_OR_EQUAL:
    return value1 >= value2;
  default:
    return false;
  }
}

bool compareStr(char *value1, char *value2, int entrySize, int operatorType) {
  // int isEqual = std::memcmp((void *)value1, (void *)value2,
  // (size_t)STRING_SIZE); compare strings using strcmp
  int isEqual = std::strcmp(value1, value2);

  switch (operatorType) {
  case EQUAL:
    return isEqual == 0;
  case NOT_EQUAL:
    return isEqual != 0;
  case LESS_THAN:
    return isEqual < 0;
  case GREATER_THAN:
    return isEqual > 0;
  case LESS_THAN_OR_EQUAL:
    return isEqual <= 0;
  case GREATER_THAN_OR_EQUAL:
    return isEqual >= 0;
  default:
    return false;
  }
}

bool isStatementMultithread(hsql::StatementType type) {
  switch (type) {
  case hsql::kStmtCreate:
    return false;
    break;

  case hsql::kStmtInsert:
    return false;
    break;

  default:
    return true;
    break;
  }
}
void parseSelectStatement(const hsql::SQLStatement *statement) {
  if (statement == NULL) {
    return;
  }

  // SELECT Products.ProductID, Products.ProductName, Categories.CategoryName
  // FROM Products INNER JOIN Categories ON Products.CategoryID =
  // Categories.CategoryID; select statement
  if (statement->type() == hsql::kStmtSelect) {
    const auto *selectStatement =
        static_cast<const hsql::SelectStatement *>(statement);

    // if statement is a join statement
    if (selectStatement->fromTable != nullptr &&
        selectStatement->fromTable->type == hsql::kTableJoin) {
      const auto *joinTable = selectStatement->fromTable->join;
      std::cout << "Join Table: " << joinTable->left->name << " "
                << joinTable->right->name << std::endl;
      std::cout << "Join Condition Operator Type: "
                << joinTable->condition->opType << std::endl;
    }
    // Parse columns
    for (const auto *column : *selectStatement->selectList) {
      std::cout << "Column: ";
      if (column->type == hsql::kExprStar) {
        std::cout << "*";
      } else if (column->type == hsql::kExprLiteralInt) {
        std::cout << column->ival;
      } else if (column->type == hsql::kExprLiteralString) {
        std::cout << column->name;
      } else {
        std::cout << "Else  : " << column->name;
      }
      if (column->type == hsql::kExprColumnRef) {
      }
    }
    std::cout << std::endl;

    if (selectStatement->fromTable != nullptr) {
      std::cout << "Table: " << selectStatement->fromTable->name << std::endl;
    }

    if (selectStatement->whereClause != nullptr) {
      std::cout << "WHERE: \n";
      hsql::printExpression(selectStatement->whereClause, 1);
    }

    if (selectStatement->setOperations != nullptr) {
      // For each entry in the setOperation list
      for (const auto *setOperation : *(selectStatement->setOperations)) {
        std::cout << "Set Operation: ";
        if (setOperation->setType == hsql::kSetUnion) {
          std::cout << "UNION ";
        } else if (setOperation->setType == hsql::kSetExcept) {
          std::cout << "EXCEPT ";
        } else if (setOperation->setType == hsql::kSetIntersect) {
          std::cout << "INTERSECT ";
        }
        if (setOperation->isAll) {
          std::cout << "ALL ";
        }

        // print the nested select statement if it exists
        if (setOperation->nestedSelectStatement != nullptr) {
          std::cout << "Entering Nested Select: \n";
          // parseSelectStatement(setOperation->nestedSelectStatement);
          std::cout << "Exiting Nested Select\n";
        }
      }
    }
  }

  // create table statement
  if (statement->type() == hsql::kStmtCreate) {
    const auto *createStatement =
        static_cast<const hsql::CreateStatement *>(statement);
    // std::cout << "Table Name: " << createStatement->tableName << std::endl;
    // std::cout << "Columns: ";
    // for (const auto* column : *createStatement->columns) {
    //     std::cout << column->name << " ";
    // }
    // std::cout << std::endl;
  }
}
