#include "loop_functions.h"

bool fn_select_loop(LoopFnArgs args) {

  // function identifier
  int fnIdentifier = SELECT_FN_IDENTIFER;
  ReadTupleStream *input_ts = args.selectArgs.input_ts;

  void *input_tuple = input_ts->yieldNext();
  if (input_tuple == NULL) {
    return false;
  }
  // get selected attribute from the input tuple stream
  Attribute *selectedAttrFromInputForCompare;
  for (auto attr : input_ts->getAttributeList()) {
    if (attr.name == args.selectArgs.attribute->name) {
      selectedAttrFromInputForCompare = &attr;
      break;
    }
  }
  // get the value offset of the columns needed for comparison
  // the output tuple stream already contains the nessesarily modified
  // attributes listor for the output tuple
  Tuple tup;
  void *input_tuple_val =
      tup.getTupleValue(selectedAttrFromInputForCompare, input_tuple);

  // check if the input tuple matches the condition
  bool matched = Operator::match(
      input_tuple_val, args.selectArgs.matchVal, args.selectArgs.op,
      args.selectArgs.attribute->type, args.selectArgs.entrySize);

  // if no match then return
  if (not matched) {
    return true;
  }

  // if match then insert the new value into the output tuple stream
  WriteTupleStream *output_ts = args.selectArgs.output_ts;

  // modify the input tuple to contain only the selected attribute
  void *output_tuple = (void *)malloc(sizeof(char) * output_ts->getEntrySize());

  // copy the selected attribute value to the output tuple
  for (auto attr1 : output_ts->getAttributeList()) {
    // if the attribute is the selected attribute then copy the value to the
    // output tuple
    for (auto attr2 : input_ts->getAttributeList()) {
      if (attr1.name == attr2.name) {
        memcpy((char *)output_tuple + attr1.offset,
               (char *)input_tuple + attr2.offset, attr1.size);
        break;
      }
    }
  }

  output_ts->insert(output_tuple);
  free(output_tuple);

  return true;
}

// left = probe
// right = build

bool fn_join_loop(LoopFnArgs args) {
  // function identifier
  int fnIdentifier = JOIN_FN_IDENTIFIER;
  ReadTupleStream *probe_table_ts = args.joinArgs.probe_ts;

  // build table input tuple is the tuple that is being compared with the probe
  // table build table is the smaller table probe table is the larger table
  void *probe_input_tuple = probe_table_ts->yieldNext();
  if (probe_input_tuple == NULL) {
    return false;
  }

  RelationCatalogEntry *buildTableEntry = new RelationCatalogEntry();

  RelationCatalog RELCAT;
  int ret =
      RELCAT.getTableEntry(*(args.joinArgs.buildTableName), buildTableEntry);

  // if the table is not found
  if (ret != 0) {
    std::cout << "Table not found\n";
    return -1;
  }

  StaticVars staticVar;

  for (int probing_core = 1; probing_core <= staticVar.getNumberOfCores();
       probing_core++) {
    ReadTupleStream *build_table_input_ts =
        new ReadTupleStream(buildTableEntry, probing_core);

    while (true) {
      void *build_input_tuple = build_table_input_ts->yieldNext();
      if (build_input_tuple == NULL) {
        break;
      }

      Tuple tup;
      void *build_input_tuple_val =
          tup.getTupleValue(args.joinArgs.buildTableAttr, build_input_tuple);
      void *probe_input_tuple_val =
          tup.getTupleValue(args.joinArgs.probeTableAttr, probe_input_tuple);

      bool matched = Operator::match(
          build_input_tuple_val, probe_input_tuple_val, args.joinArgs.op,
          args.joinArgs.probeTableAttr->type, args.joinArgs.entrySize);
      WriteTupleStream *output_ts = args.joinArgs.output_ts;
      void *output_tuple =
          (void *)malloc(sizeof(char) * output_ts->getEntrySize());
      if (matched) {

        for (const auto &attr1 : output_ts->getAttributeList()) {
          bool hit = false;
          // if the attribute is the selected attribute then copy the value to
          // the output tuple
          for (const auto &attr2 : probe_table_ts->getAttributeList()) {
            if (attr1.name == attr2.name) {
              hit = true;
              memcpy((char *)output_tuple + attr1.offset,
                     (char *)probe_input_tuple + attr2.offset, attr1.size);
            }
          }

          if (hit == false) {
            for (const auto &attr2 : build_table_input_ts->getAttributeList()) {
              if (attr1.name == attr2.name) {
                hit = true;
                memcpy((char *)output_tuple + attr1.offset,
                       (char *)build_input_tuple + attr2.offset, attr1.size);
              }
            }
          }
        }

        output_ts->insert(output_tuple);
      }

      free(output_tuple);
    }

    delete build_table_input_ts;
  }

  delete buildTableEntry;
  return true;
}