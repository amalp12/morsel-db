#include "dispatcher.h"
#include "relcat.h"
#include <chrono>
#include "loop_functions.h"

const hsql::SQLStatement *QEP::statement;
std::map<hsql::Expr *, std::array<int, 48>> QEP::dependencyMap;
hsql::StatementType QEP::statementType;
std::queue<MorselContainer> QEP::qepQueue;
// Dispatcher DISPATCHER;
// QEP QEPObject ;
// SELECT table1.column1,table1.column2,table2.column1 FROM table1 INNER JOIN table2 ON table1.matching_column = table2.matching_column;
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

int QEP::execute(int coreNum) {
  // Get the current time before running the program
  // auto start_time = std::chrono::high_resolution_clock::now();
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
  // declare start time type


  // if the statement is select
  if (statementType == hsql::kStmtSelect) {
    // declare lists to store columsn table and where clause
    std::string table;
    std::string whereClause;
    Attribute lhsAttr;

    const hsql::SelectStatement *selectStatement =
        static_cast<const hsql::SelectStatement *>(statement);

    auto tableName = selectStatement->fromTable != nullptr
                         ? selectStatement->fromTable->name
                         : NULL;
    start_time = std::chrono::high_resolution_clock::now();
    // get relationcat entry
    RelationCatalogEntry *entry = new RelationCatalogEntry();

    RelationCatalog RELCAT;
    int ret = RELCAT.getTableEntry(tableName, entry);

    // if the table is not found
    if (ret != 0) {
      std::cout << "Table not found\n";
      return -1;
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

      std::string tempTableName="_" + entry->getTableName();
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

      // ggfn_select_loop(args);

      // print the input tuple stream for debugging
      // args.selectArgs.input_ts->printStream();

      Operator::loop(fn_select_loop, args);

      // print the output tuple stream for debugging
      // args.selectArgs.output_ts->printStream();

      std::string output_file_name = "/home/ssl/morsel-db/out/output_" +
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
  }

  else if(statementType == hsql::kStmtCreate)
  {
    RelationCatalog relCat;
    const hsql::CreateStatement *createStatement = 
        static_cast<const hsql::CreateStatement *>(statement);

    std::string tableName = createStatement->tableName;
    std::list<std::string> colNameList;
    std::list<int>  colTypeList;

    start_time = std::chrono::high_resolution_clock::now();

    for(const auto *column : *createStatement->columns)
    {
      colNameList.emplace_back(column->name);
    
      if(column->type == hsql::DataType::INT)
       colTypeList.emplace_back(INTEGER);

      else if(column->type == hsql::DataType::VARCHAR)
       colTypeList.emplace_back(STRING);
      else if(column->type == hsql::DataType::TEXT)
       colTypeList.emplace_back(STRING);
      else
        std::cout << column << '\n';
      
    
    }
    
    relCat.insertNewTable(tableName , colNameList , colTypeList);
  }


  else if(statementType == hsql::kStmtInsert){

    RelationCatalog relCat;

    const hsql::InsertStatement* insertStatement = 
        static_cast<const hsql::InsertStatement*>(statement);
    
    std::string tableName = insertStatement->tableName;

    start_time = std::chrono::high_resolution_clock::now();

    RelationCatalogEntry* entry = new RelationCatalogEntry();
    
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
    for (const auto& attribute : attributeList) {
      recordSize += attribute.size;
    }
    
    // Allocate memory for destination
    char* destination = new char[recordSize]; // static_cast<char*>(malloc(recordSize));
    int insertStatementIndex=0;
    for (auto iter = attributeList.begin() ; iter != attributeList.end(); ++iter) {
      if ((*iter).type == INTEGER) {
        int value = (*insertStatement->values)[insertStatementIndex]->ival;
        memcpy(destination + (*iter).offset, &value, INTEGER_SIZE);
      } else if ((*iter).type == STRING) {
        const char* strValue = (*insertStatement->values)[insertStatementIndex]->name;
        memcpy(destination + (*iter).offset, strValue, STRING_SIZE);
      }
      insertStatementIndex++;
    }
    
    // Append to thread map morsel
    relCat.appendToThreadMapMorsel(tableName, coreNum, destination);
    
    // Clean up allocated memory
    delete[] destination;
    delete entry;

  }

  // Get the current time after running the program
  auto end_time = std::chrono::high_resolution_clock::now();
  // Calculate the duration in milliseconds
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);
  // Print the time taken
  // std::cout << "Time taken: " << duration.count() << " milliseconds"
  //           << std::endl;

  return duration.count();
  // return 0;
}

int Dispatcher::execute(int coreNum) { return 0; }

int Dispatcher::getCapacity() { return capacity; }

int Dispatcher::setCapacity(int capacity) {
  this->capacity = capacity;
  return 0;
}
