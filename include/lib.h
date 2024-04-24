#pragma once
#include <hsql/util/sqlhelper.h>
#include <string>
// #include <hsql/SQLParser.h>

bool compareInt(int value1, int value2, int operatorType);
bool compareStr(char *value1, char *value2, int entrySize, int operatorType);
bool isStatementMultithread(hsql::StatementType type);
void parseSelectStatement(const hsql::SQLStatement *statement);
int compareAttrs(int val1, int val2, int operatorType);
int compareAttrs(std::string val1, std::string val2, int operatorType);