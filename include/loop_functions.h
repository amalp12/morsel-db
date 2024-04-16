#pragma once
#include "tuple.h"
#include "operators.h"

struct SelectFn
{ 
  ReadTupleStream * input_ts;
  WriteTupleStream * output_ts;
  // match
  void * matchVal;
  Attribute * attribute;
  int op;
  int entrySize;

};


union LoopFnArgs{
    struct SelectFn selectArgs;
};

bool fn_select_loop(union LoopFnArgs args);