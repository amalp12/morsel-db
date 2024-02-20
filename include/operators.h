#pragma once
#include "constants.h"
#include "tuple.h"
#include "lib.h"
#include <functional>
#include "loop_functions.h"

class Operator {

    public:
    // scan
    static TupleStream * scan(Morsel *tableHeadMorsel, std::vector<std::string> colNames,
                            std::vector<int> colTypes);

    // match
    static bool match(void *value1, void *value2, int operatorType , int valueType, int entrySize);

    // filter
    static void * filter(void * tuple, TupleStream * ts, std::vector<std::string> selectList);

    // scatter
    static int scatter(void * tuple, Morsel * morsel, std::vector<Attribute> attributeList);

    //loop : for each tuple in the tuple stream, apply the function that returns a bool until it returns false
    static void loop(std::function<bool(union LoopFnArgs args)>  fn, union LoopFnArgs args);
    
    // map
};

