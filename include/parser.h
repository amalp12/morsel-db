#pragma once
#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <iostream>
#include <pg_query.h>

void parseSelectStatement(const hsql::SQLStatement* statement);