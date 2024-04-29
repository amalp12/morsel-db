#pragma once
#include <hsql/util/sqlhelper.h>

bool compareInt(int value1, int value2, int operatorType);
bool compareStr(char *value1, char *value2, int entrySize, int operatorType);
bool isStatementMultithread(hsql::StatementType type);
void parseSelectStatement(const hsql::SQLStatement *statement);
int compareAttrs(int *val1, int *val2);
int compareAttrs(char *val1, char *val2);
