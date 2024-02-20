#include "operators.h"

TupleStream *Operator::scan(Morsel *tableHeadMorsel,
                            std::vector<std::string> colNames,
                            std::vector<int> colTypes) {
  return new TupleStream(tableHeadMorsel, colNames, colTypes);
}


bool Operator::match(void *value1, void *value2, int operatorType , int valueType, int entrySize) {
  switch (valueType) {
  case INTEGER:
    return compareInt(*(int *)value1 , *(int *)value2 , operatorType);
  case STRING:
    return compareStr((char *)value1 , (char*)value2 , entrySize , operatorType);
  default:
    return false;
  }
}



int Operator::scatter(void *tuple, Morsel *morsel , std::vector<Attribute> attributeList) {
    return morsel->insertEntry(tuple , attributeList);
}

void Operator::loop(std::function<bool(union LoopFnArgs args)> fn, union LoopFnArgs args){
  // reset the tuple stream
  args.selectArgs.input_ts->resetStream();

  bool ret;
  do{
    ret = fn(args);
  }while(ret);

}

// void *  Operator::filter(void * tuple, TupleStream * ts, std::vector<Attribute> selectList, Morsel * outputMorsel){
// 	// get the  attribute vector of the tuple stream
//   std::vector<Attribute> allAttributeList = ts->getAttributeList();

//   // for each attribute in the select vector, get the value from the tuple



// }



