#include "loop_functions.h"

bool fn_select_loop(LoopFnArgs args)
{
    
    // function identifier
    int fnIdentifier = SELECT_FN_IDENTIFER;
    ReadTupleStream * input_ts = args.selectArgs.input_ts;

    void * input_tuple = input_ts->yieldNext();
    if (input_tuple == NULL)
    {
        return false;
    }
    // get selected attribute from the input tuple stream
    Attribute * selectedAttrFromInputForCompare ;
    for(auto attr : input_ts->getAttributeList()){
        if(attr.name == args.selectArgs.attribute->name){
            selectedAttrFromInputForCompare = &attr;
            break;
        }
    }
    // get the value offset of the columns needed for comparison
        // the output tuple stream already contains the nessesarily modified attributes listor for the output tuple
    Tuple tup;
    void * input_tuple_val = tup.getTupleValue(selectedAttrFromInputForCompare, input_tuple);
    
    // check if the input tuple matches the condition
    bool matched = Operator::match(input_tuple_val, args.selectArgs.matchVal, args.selectArgs.op ,args.selectArgs.attribute->type ,args.selectArgs.entrySize);

    // if no match then return
    if(not matched ){
        return true;
    }
    

    // if match then insert the new value into the output tuple stream
    WriteTupleStream * output_ts = args.selectArgs.output_ts;

    // modify the input tuple to contain only the selected attribute
    void * output_tuple = (void *) malloc(sizeof(char)*output_ts->getEntrySize());

    // copy the selected attribute value to the output tuple
    for (auto attr1 : output_ts->getAttributeList())
    {
        // if the attribute is the selected attribute then copy the value to the output tuple
        for(auto attr2 : input_ts->getAttributeList()){
            if(attr1.name == attr2.name){
                memcpy((char*)output_tuple + attr1.offset, (char*)input_tuple + attr2.offset, attr1.size);
                break;
            }
        }
    }
    
    output_ts->insert(output_tuple);
    free(output_tuple);

    

    return true;

}