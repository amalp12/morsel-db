#include "parser.h"

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