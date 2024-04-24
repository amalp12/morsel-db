#pragma once
#include "operators.h"
#include "tuple.h"

struct SelectFn {
  ReadTupleStream *input_ts;
  WriteTupleStream *output_ts;
  // match
  void *matchVal;
  Attribute *attribute;
  int op;
  int entrySize;
};

struct JoinFn {
  ReadTupleStream *probe_ts;
  WriteTupleStream *output_ts;
  Attribute *probeTableAttr;
  Attribute *buildTableAttr;
  std::string *buildTableName;
  int op;
  int entrySize;
  int coreNum;
};

union LoopFnArgs {
  struct SelectFn selectArgs;
  struct JoinFn joinArgs;
};

bool fn_select_loop(union LoopFnArgs args);
bool fn_join_loop(LoopFnArgs args);
void *linearSearch(LoopFnArgs args, void *probe_input_tuple,
                   ReadTupleStream *build_table_input_ts);