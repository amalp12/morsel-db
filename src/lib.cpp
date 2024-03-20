#include "lib.h"

bool compareInt(int value1 , int value2 , int operatorType)
{
      switch (operatorType) {
      case EQUAL:
        return value1 ==value2;
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


bool compareStr(char * value1 , char * value2 , int entrySize , int operatorType)
{
  int isEqual = std::memcmp((void *) value1 ,(void *) value2 , (size_t)entrySize);


  switch (operatorType) {
      case EQUAL:
        return isEqual==0;
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


}}

bool isStatementMultithread(hsql::StatementType type)
{
  switch (type)
  {
  case hsql::kStmtCreate :
    return false;
    break;

  case hsql::kStmtInsert :
    return false;
    break;
  
  default:
    return true;
    break;
  }
}
void parseSelectStatement(const hsql::SQLStatement* statement) {
    // select statement
    if (statement->type() == hsql::kStmtSelect) {
        const auto* selectStatement = static_cast<const hsql::SelectStatement*>(statement);
        // Parse columns
        for (const auto* column : *selectStatement->selectList) {
            if (column->type == hsql::kExprColumnRef) {
            }
        }
        // std::cout << std::endl;

        // if (selectStatement->fromTable != nullptr) {
        //     std::cout << "Table: " << selectStatement->fromTable->name << std::endl;
        // }

        // if (selectStatement->whereClause != nullptr) {
        //     std::cout << "WHERE: \n"; ;hsql::printExpression(selectStatement->whereClause, 1);
        // }
    }

    // create table statement
    if (statement->type() == hsql::kStmtCreate) {
        const auto* createStatement = static_cast<const hsql::CreateStatement*>(statement);
        // std::cout << "Table Name: " << createStatement->tableName << std::endl;
        // std::cout << "Columns: ";
        // for (const auto* column : *createStatement->columns) {
        //     std::cout << column->name << " ";
        // }
        // std::cout << std::endl;
    }
}