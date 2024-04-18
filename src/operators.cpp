#include "operators.h"
#include "lib.h"

TupleStream *Operator::scan(Morsel *tableHeadMorsel,
                            std::list<std::string> colNames,
                            std::list<int> colTypes)
{
  return new TupleStream(tableHeadMorsel, colNames, colTypes);
}

bool Operator::match(void *value1, void *value2, int operatorType, int valueType, int entrySize)
{
  switch (valueType)
  {
  case INTEGER:
    return compareInt(*(int *)value1, *(int *)value2, operatorType);
  case STRING:
    return compareStr((char *)value1, (char *)value2, entrySize, operatorType);
  default:
    return false;
  }
}

int Operator::scatter(void *tuple, Morsel *morsel, std::list<Attribute> attributeList)
{
  return morsel->insertEntry(tuple, attributeList);
}

void Operator::loop(std::function<bool(union LoopFnArgs args)> fn, union LoopFnArgs args, int functionIdentifier)
{

  if (functionIdentifier == SELECT_FN_IDENTIFER )
  {
    // reset the tuple stream
    args.selectArgs.input_ts->resetStream();

    bool ret;
    do
    {
      ret = fn(args);
    } while (ret);
  }

  if(functionIdentifier == JOIN_FN_IDENTIFIER)
  {
    // reset the tuple stream
    args.joinArgs.probe_ts->resetStream();
    bool ret;
    do
    {
      ret = fn(args);
    } while (ret);
  }
}

// void *  Operator::filter(void * tuple, TupleStream * ts, std::list<Attribute> selectList, Morsel * outputMorsel){
// 	// get the  attribute list of the tuple stream
//   std::list<Attribute> allAttributeList = ts->getAttributeList();

//   // for each attribute in the select list, get the value from the tuple

// }
