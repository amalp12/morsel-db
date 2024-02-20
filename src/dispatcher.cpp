#include "dispatcher.h"

const hsql::SQLStatement * QEP::statement;
std::map<hsql::Expr*, std::array<int, NUMBER_OF_CORES>> QEP::dependencyMap;
hsql::StatementType QEP::statementType;
std::queue<MorselContainer> QEP::qepQueue;
// Dispatcher DISPATCHER;
// QEP QEPObject ;

int QEP::assignDependancies(int coreNum) {
  if (statementType == hsql::kStmtSelect) {
    const hsql::SelectStatement *selectStatement =
        static_cast<const hsql::SelectStatement *>(statement);

    // assigning dependancies

    // where clause
    if (selectStatement->whereClause != NULL) {
      dependencyMap[selectStatement->whereClause][coreNum] = 1;
    }
    // select vector
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
QEP::QEP(const hsql::SQLStatement * statement){
        this->statement = statement;
        this->statementType = statement->type();
}

QEP::QEP(){
    this->statement = NULL;
    this->statementType = hsql::kStmtError;
}

int QEP::execute(int coreNum) {
  // if the statement is select
  if (statementType == hsql::kStmtSelect) {
    // declare vectors to store columsn table and where clause
    std::string table;
    std::string whereClause;
    Attribute lhsAttr;

    const hsql::SelectStatement *selectStatement =
        static_cast<const hsql::SelectStatement *>(statement);

        auto tableName = selectStatement->fromTable != nullptr?selectStatement->fromTable->name:NULL;
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
        auto columnsVector = entry->getAttributes();
        
        // create a vector for the selected columns
        std::vector<std::string> selectedColNameList;
        std::vector<int> selectedColTypeList;

        // Parse columns
        for (const auto* column : *selectStatement->selectList) {
            if (column->type == hsql::kExprColumnRef) {
              //  std::cout << column->name << " ";

              // search the columnsVector for the column name and append that Attribute to the selectedColumnsVector
              for (auto attribute : columnsVector) {
                if (attribute.name == column->name) {
                  selectedColNameList.push_back(attribute.name);
                  selectedColTypeList.push_back(attribute.type);
                }
              }
            }
        }

       
      std::cout << "Entering Select ... " <<std::endl;
        if (selectStatement->whereClause != nullptr) {
            std::cout << "WHERE: \n"; ;hsql::printExpression(selectStatement->whereClause, 1);

            // get the rhs, lhs and the operator fromt the where clause
            auto lhs = selectStatement->whereClause->expr;
            auto rhs = selectStatement->whereClause->expr2;
            auto op = selectStatement->whereClause->opType;

            // print the rhs, lhs and the operator
            std::cout << "LHS: " << lhs->name << std::endl;
            std::cout << "RHS: " << rhs->ival << std::endl;
            std::cout << "OP: " << op << std::endl;

            

            // if the table is not found
            if (ret != 0) {
              std::cout << "Table not found\n";
              return -1;
            }

            // get entire table column attributes
            auto columnsVector = entry->getAttributes();

            //get attribute of LHS
            for (auto attribute : columnsVector) {
                if (attribute.name == lhs->name) {
                  lhsAttr = attribute;
                }
              }

            // create a new table
    RelationCatalog relCat;
    // SELECT Name FROM test_table WHERE Age > 25;

    relCat.insertNewTable("_"+entry->getTableName(),selectedColNameList, selectedColTypeList);

    RelationCatalogEntry *newEntry = new RelationCatalogEntry();
    ret = RELCAT.getTableEntry("_"+entry->getTableName(), newEntry);

            // call seleect fn loop
            union LoopFnArgs args;

            args.selectArgs.input_ts = new ReadTupleStream(entry, coreNum);

            args.selectArgs.output_ts = new WriteTupleStream(newEntry, coreNum);

            // allocate memory for matchval 
            int * matchValMem= (int *) malloc(sizeof(int));
            // copy rhs to matchva
            (*matchValMem)=rhs->ival;

        




            args.selectArgs.matchVal = matchValMem;
            args.selectArgs.attribute = &(lhsAttr);
            args.selectArgs.op = hsql::kOpGreater?GREATER_THAN:LESS_THAN;
            args.selectArgs.entrySize = lhsAttr.size;
            

            // fn_select_loop(args);

            // print the input tuple stream for debugging
            args.selectArgs.input_ts->printStream();

            Operator::loop(fn_select_loop, args);

            // print the output tuple stream for debugging
            std::string output_file_name = "/home/ssl/Code/db/out/output_"+entry->getTableName()+"_"+std::to_string(coreNum)+".txt";
            args.selectArgs.output_ts->writeStream(output_file_name);

            // peda gdb
        
            

        }
        std::cout << "Exiting Select ... " <<std::endl;


  }
  return 0;
}




int Dispatcher::execute(int coreNum){
    return 0;
}

int Dispatcher::getCapacity(){
    return capacity;
}

int Dispatcher::setCapacity(int capacity){
    this->capacity = capacity;
    return 0;
}
