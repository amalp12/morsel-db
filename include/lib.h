#pragma once
#include <cstring>
#include <hsql/util/sqlhelper.h>
// #include <hsql/SQLParser.h>

bool compareInt(int value1, int value2, int operatorType);
bool compareStr(char *value1, char *value2, int entrySize, int operatorType);
bool isStatementMultithread(hsql::StatementType type);
void parseSelectStatement(const hsql::SQLStatement *statement);