#include "dispatcher.h"
#include "loop_functions.h"
#include "relcat.h"
#include "test.h"
#include <chrono>

const hsql::SQLStatement *QEP::statement;
std::map<hsql::Expr *, std::array<int, 48>> QEP::dependencyMap;
hsql::StatementType QEP::statementType;
std::queue<MorselContainer> QEP::qepQueue;
// Dispatcher DISPATCHER;
// QEP QEPObject ;
// SELECT table1.column1,table1.column2,table2.column1 FROM table1 INNER JOIN
// table2 ON table1.matching_column = table2.matching_column;
int QEP::assignDependancies(int coreNum) {
  if (statementType == hsql::kStmtSelect) {
    const hsql::SelectStatement *selectStatement =
        static_cast<const hsql::SelectStatement *>(statement);

    // assigning dependancies

    // where clause
    if (selectStatement->whereClause != NULL) {
      dependencyMap[selectStatement->whereClause][coreNum] = 1;
    }
    // select list
    for (auto expr : *(selectStatement->selectList)) {
      dependencyMap[expr][coreNum] = 1;
    }
    return 0;
    // from table
    /*TODO: make nested select statements in from table work*/
  }

  // create local morsel
  return 0;
}

QEP::QEP(const hsql::SQLStatement *statement) {
  this->statement = statement;
  this->statementType = statement->type();
}

QEP::QEP() {
  this->statement = NULL;
  this->statementType = hsql::kStmtError;
}

int QEP::handleCreateTable(int coreNum) {
  RelationCatalog relCat;
  const hsql::CreateStatement *createStatement =
      static_cast<const hsql::CreateStatement *>(statement);

  std::string tableName = createStatement->tableName;
  std::list<std::string> colNameList;
  std::list<int> colTypeList;

  // start_time = std::chrono::high_resolution_clock::now();

  for (const auto *column : *createStatement->columns) {
    colNameList.emplace_back(column->name);

    if (column->type == hsql::DataType::INT)
      colTypeList.emplace_back(INTEGER);

    else if (column->type == hsql::DataType::VARCHAR)
      colTypeList.emplace_back(STRING);
    else if (column->type == hsql::DataType::TEXT)
      colTypeList.emplace_back(STRING);
    else
      std::cout << column << '\n';
  }

  return relCat.insertNewTable(tableName, colNameList, colTypeList);
}

int QEP::handleNameSelect(int coreNum,
                          const hsql::SelectStatement *selectStatement) {
  // declare lists to store columsn table and where clause
  std::string table;
  std::string whereClause;
  Attribute lhsAttr;

  auto tableName = selectStatement->fromTable != nullptr
                       ? selectStatement->fromTable->name
                       : NULL;
  // start_time = std::chrono::high_resolution_clock::now();
  // get relationcat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  RelationCatalog RELCAT;
  int ret = RELCAT.getTableEntry(tableName, entry);

  // if the table is not found
  if (ret != 0) {
    std::cout << "Table not found\n";
    return FAILURE;
  }

  // get entire table column attributes
  auto columnslist = entry->getAttributes();

  // create a list for the selected columns
  std::list<std::string> selectedColNameList;
  std::list<int> selectedColTypeList;

  // Parse columns
  for (const auto &column : *(selectStatement->selectList)) {
    if (column->type == hsql::kExprColumnRef) {
      //  std::cout << column->name << " ";

      // search the columnslist for the column name and append that
      // Attribute to the selectedColumnslist
      for (auto &attribute : columnslist) {
        if (attribute.name == column->name) {
          selectedColNameList.emplace_back(attribute.name);
          selectedColTypeList.emplace_back(attribute.type);
        }
      }
    }
  }

  // std::cout << "Entering Select ... " <<std::endl;
  if (selectStatement->whereClause != nullptr) {
    // std::cout << "WHERE: \n";
    // ;hsql::printExpression(selectStatement->whereClause, 1);

    // get the rhs, lhs and the operator fromt the where clause
    auto lhs = selectStatement->whereClause->expr;
    auto rhs = selectStatement->whereClause->expr2;
    auto op = selectStatement->whereClause->opType;

    // print the rhs, lhs and the operator
    // std::cout << "LHS: " << lhs->name << std::endl;
    // std::cout << "RHS: " << rhs->ival << std::endl;
    // std::cout << "OP: " << op << std::endl;

    // if the table is not found
    if (ret != 0) {
      std::cout << "\n";
      return -1;
    }

    // get entire table column attributes
    const auto columnslist = entry->getAttributes();

    // get attribute of LHS
    for (auto &attribute : columnslist) {
      if (attribute.name == lhs->name) {
        lhsAttr = attribute;
      }
    }

    // create a new table
    RelationCatalog relCat;
    // SELECT ID,Name,Age FROM test_table WHERE Age > 0;

    std::string tempTableName = "_" + entry->getTableName();
    relCat.insertNewTable(tempTableName, selectedColNameList,
                          selectedColTypeList);

    RelationCatalogEntry *newEntry = new RelationCatalogEntry();
    ret = RELCAT.getTableEntry(tempTableName, newEntry);

    // call seleect fn loop
    union LoopFnArgs args;

    args.selectArgs.input_ts = new ReadTupleStream(entry, coreNum);

    args.selectArgs.output_ts = new WriteTupleStream(newEntry, coreNum);

    // allocate memory for matchval
    int64_t matchValMem = rhs->ival;
    // copy rhs to matchva
    args.selectArgs.matchVal = &matchValMem;
    args.selectArgs.attribute = &(lhsAttr);
    args.selectArgs.op = hsql::kOpGreater ? GREATER_THAN : LESS_THAN;
    args.selectArgs.entrySize = lhsAttr.size;

    // fn_select_loop(args);

    // print the input tuple stream for debugging
    // args.selectArgs.input_ts->printStream();

    Operator::loop(fn_select_loop, args, SELECT_FN_IDENTIFER);

    // print the output tuple stream for debugging
    // args.selectArgs.output_ts->printStream();

    std::string output_file_name = get_env_var("ROOTDIR") + "/out/output_" +
                                   entry->getTableName() + "_" +
                                   std::to_string(coreNum) + ".txt";
    // args.selectArgs.output_ts->writeStream(output_file_name);

    // peda gdb

    delete entry;
    delete newEntry;
    // delete input_ts and output_ts
    delete args.selectArgs.input_ts;
    delete args.selectArgs.output_ts;
  }
  // std::cout << "Exiting Select ... " <<std::endl;
  return SUCCESS;
}

int QEP::handleJoin(int coreNum, const hsql::SelectStatement *selectStatement) {
  // join statement
  auto joinStatement = selectStatement->fromTable->join;
  // operator type
  auto opType = joinStatement->condition->opType;
  // left table
  auto leftTableName = joinStatement->left->name;
  // right table
  auto rightTableName = joinStatement->right->name;
  std::string rightTableNameStr(rightTableName);
  // left expression
  auto leftColumnName = joinStatement->condition->expr->name;
  // left expression type
  // auto leftExprType = leftExpr->type;

  // right expression
  auto rightColumnName = joinStatement->condition->expr2->name;
  // right expression type
  // auto rightExprType = rightExpr->type;

  RelationCatalog RELCAT;

  // get left table entry
  RelationCatalogEntry *leftTableEntry = new RelationCatalogEntry();
  int ret = RELCAT.getTableEntry(leftTableName, leftTableEntry);

  // if the table is not found
  if (ret != 0) {
    std::cout << "Table not found\n";
    return FAILURE;
  }

  // get right table entry
  RelationCatalogEntry *rightTableEntry = new RelationCatalogEntry();
  ret = RELCAT.getTableEntry(rightTableName, rightTableEntry);

  // if the table is not found
  if (ret != 0) {
    std::cout << "Table not found\n";
    return FAILURE;
  }

  // create a list for the selected columns
  std::list<std::string> selectedColNameList;
  std::list<int> selectedColTypeList;

  auto columnsListLeft = leftTableEntry->getAttributes();
  auto columnsListRight = rightTableEntry->getAttributes();

  // Parse columns
  for (const auto &column : *(selectStatement->selectList)) {
    bool hit = false;
    if (column->type == hsql::kExprColumnRef) {
      //  std::cout << column->name << " ";

      // search the columnslist for the column name and append that
      // Attribute to the selectedColumnslist
      for (auto &attribute : columnsListLeft) {
        if (attribute.name == column->name) {
          selectedColNameList.emplace_back(attribute.name);
          selectedColTypeList.emplace_back(attribute.type);
          hit = true;
        }
      }

      if (hit == false) {
        for (auto &attribute : columnsListRight) {
          if (attribute.name == column->name) {
            selectedColNameList.emplace_back(attribute.name);
            selectedColTypeList.emplace_back(attribute.type);
            hit = true;
          }
        }
      }
    }
  }

  Attribute *leftAttr;
  Attribute *rightAttr;

  for (auto &attribute : columnsListLeft) {
    if (attribute.name == leftColumnName) {
      leftAttr = &(attribute);
    }
  }

  for (auto &attribute : columnsListRight) {
    if (attribute.name == rightColumnName) {
      rightAttr = &(attribute);
    }
  }

  std::string tempTableName = "temp_join_result";
  RELCAT.insertNewTable(tempTableName, selectedColNameList,
                        selectedColTypeList);

  RelationCatalogEntry *newEntry = new RelationCatalogEntry();
  ret = RELCAT.getTableEntry(tempTableName, newEntry);

  if (ret != 0) {
    std::cout << "Table Not Found!";
    return FAILURE;
  }

  // call seleect fn loop
  union LoopFnArgs args;

  args.joinArgs.probe_ts = new ReadTupleStream(leftTableEntry, coreNum);

  args.joinArgs.output_ts = new WriteTupleStream(newEntry, coreNum);

  args.joinArgs.probeTableAttr = leftAttr;
  args.joinArgs.buildTableAttr = rightAttr;
  args.joinArgs.buildTableName = &rightTableNameStr;
  args.joinArgs.entrySize = args.joinArgs.output_ts->getEntrySize();
  switch (opType) {
  case hsql::kOpEquals:
    args.joinArgs.op = EQUAL;
    break;

  case hsql::kOpGreater:
    args.joinArgs.op = GREATER_THAN;

  case hsql::kOpGreaterEq:
    args.joinArgs.op = GREATER_THAN_OR_EQUAL;
    break;

  case hsql::kOpLess:
    args.joinArgs.op = LESS_THAN;

  case hsql::kOpLessEq:
    args.joinArgs.op = LESS_THAN_OR_EQUAL;

  default:
    break;
  }

  Operator::loop(fn_join_loop, args, JOIN_FN_IDENTIFIER);

  std::string output_file_name = "/home/ssl/Code/db/out/temp_join_result_" +
                                 std::to_string(coreNum) + ".csv";

  args.joinArgs.output_ts->writeStream(output_file_name);
  // args.joinArgs.output_ts->printStream();

  return SUCCESS;
}

int QEP::handleSelect(int coreNum) {
  const hsql::SelectStatement *selectStatement =
      static_cast<const hsql::SelectStatement *>(statement);

  auto selectStatementType = selectStatement->fromTable->type;

  int retVal = FAILURE;

  switch (selectStatementType) {
  case hsql::kTableName: {
    retVal = handleNameSelect(coreNum, selectStatement);
    break;
  }
  case hsql::kTableJoin: {
    // std::cout << "Join Table: " << joinTable->left->name << " " <<
    // joinTable->right->name << std::endl; std::cout << "Join Condition
    // Operator Type: " << joinTable->condition->opType << std::endl;
    retVal = handleJoin(coreNum, selectStatement);
    break;
  }
  default: {
    std::cout << "Invalid select statement type\n";
    break;
  }
  }
  return retVal;
}

int QEP::handleInsert(int coreNum) {
  RelationCatalog relCat;

  const hsql::InsertStatement *insertStatement =
      static_cast<const hsql::InsertStatement *>(statement);

  std::string tableName = insertStatement->tableName;

  // start_time = std::chrono::high_resolution_clock::now();

  RelationCatalogEntry *entry = new RelationCatalogEntry();

  int ret = relCat.getTableEntry(tableName, entry);

  // Check if the table is not found
  if (ret != 0) {
    std::cout << "Table not found (insert)\n";
    delete entry; // Clean up allocated memory
    return -1;
  }

  std::list<Attribute> attributeList = entry->getAttributes();

  // Calculate record size
  int recordSize = 0;
  for (const auto &attribute : attributeList) {
    recordSize += attribute.size;
  }

  // Allocate memory for destination
  char *destination =
      new char[recordSize]; // static_cast<char*>(malloc(recordSize));
  int insertStatementIndex = 0;
  for (auto iter = attributeList.begin(); iter != attributeList.end(); ++iter) {
    if ((*iter).type == INTEGER) {
      int value = (*insertStatement->values)[insertStatementIndex]->ival;
      memcpy(destination + (*iter).offset, &value, INTEGER_SIZE);
    } else if ((*iter).type == STRING) {
      const char *strValue =
          (*insertStatement->values)[insertStatementIndex]->name;
      memcpy(destination + (*iter).offset, strValue, STRING_SIZE);
    }
    insertStatementIndex++;
  }

  // Append to thread map morsel
  relCat.appendToThreadMapMorsel(tableName, coreNum, destination);

  // Clean up allocated memory
  delete[] destination;
  delete entry;

  return SUCCESS;
}

int QEP::execute(int coreNum) {
  // Get the current time before running the program
  // auto start_time = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  // declare start time type

  int retval = FAILURE;
  // if the statement is select
  switch (statementType) {
  case (hsql::kStmtSelect): {
    retval = handleSelect(coreNum);
    break;
  }

  case (hsql::kStmtCreate): {
    retval = handleCreateTable(coreNum);
    break;
  }
  case (hsql::kStmtInsert): {
    retval = handleInsert(coreNum);
    break;
  }
  default: {
    std::cout << "Invalid statement type\n";
    break;
  }
  }

  // Get the current time after running the program
  auto end_time = std::chrono::high_resolution_clock::now();
  // Calculate the duration in milliseconds
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  // Print the time taken
  // std::cout << "Time taken: " << duration.count() << " milliseconds"
  //           << std::endl;

  if (retval == FAILURE) {
    std::cout << "Execution failed\n";
    return FAILURE;
  }
  return duration.count();
}

int Dispatcher::execute(int coreNum) { return 0; }

int Dispatcher::getCapacity() { return capacity; }

int Dispatcher::setCapacity(int capacity) {
  this->capacity = capacity;
  return 0;
}
