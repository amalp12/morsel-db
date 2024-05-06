// #ifndef DISPATCHER_H
// #define DISPATCHER_H
#pragma once
#include "morsel.h"
#include <hsql/SQLParser.h>
#include <hsql/util/sqlhelper.h>
#include <map>
#include <queue>
#include <numa.h>

class QEP {
public:
  static const hsql::SQLStatement *statement;
  static std::map<hsql::Expr *, std::array<int, 48>> dependencyMap;
  static hsql::StatementType statementType;
  static std::queue<MorselContainer> qepQueue;

  QEP(const hsql::SQLStatement *statement);
  QEP();
  // expr
  // progress
  // single query parallism
  // divide independently
  // select : already independant
  // join : already independant
  // project : already independant
  // create table : not writable in parallel
  // enqueue to dispatcher's respective cores if there is no clashes (fill the
  // queue to maxium before switching back to dispatcher)
  static int assignDependancies(int coreNum);

  // fill independent morsel to the dispatcher queue until its filled and exit
  static int execute(int coreNum);
  static int handleNameSelect(int coreNum,
                              const hsql::SelectStatement *selectStatement);

  static int handleCreateTable(int coreNum);
  static int handleInsert(int coreNum);
  static int handleSelect(int coreNum);
  static int handleJoin(int coreNum,
                        const hsql::SelectStatement *selectStatement);
};

class Dispatcher {
private:
  static std::queue<MorselContainer> dispatcherQueue;
  int capacity;
  // queues for each core

public:
  // empty the dispatcher queue and exit
  int execute(int coreNum);
  int getCapacity();
  int setCapacity(int capacity);

  //
};

// extern Dispatcher DISPATCHER;
// extern QEP  QEPObject;
// #endif