#include "loop_functions.h"
#include "b_plus_tree.h"
#include "constants.h"
#include "static.h"
#include <string>

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
    std::cout << "Table not found kashu\n";
    return -1;
  }

  StaticVars staticVar;

  if (not(args.joinArgs.buildTableAttr->isIndexed)) {
    for (int probing_core = 1; probing_core <= staticVar.getNumberOfCores();
         probing_core++) {
      ReadTupleStream *build_table_input_ts =
          new ReadTupleStream(buildTableEntry, probing_core);

      void *build_input_tuple =
          linearSearch(args, probe_input_tuple, build_table_input_ts);

      while (build_input_tuple != nullptr) {
        void *output_tuple = (void *)malloc(
            sizeof(char) * args.joinArgs.output_ts->getEntrySize());
        for (const auto &attr1 : args.joinArgs.output_ts->getAttributeList()) {
          bool hit = false;
          // if the attribute is the selected attribute then copy the value

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
        args.joinArgs.output_ts->insert(output_tuple);
        build_input_tuple =
            linearSearch(args, probe_input_tuple, build_table_input_ts);
      }
    }
  }

  else {
    // Write code for bplus tree search
    // get the bplus tree container
    BPlusTreeContainer *bPlusTreeContainer =
        args.joinArgs.buildTableAttr->bPlusTreeContainer;

    RelationCatalogEntry *buildTableEntry = new RelationCatalogEntry();

    RelationCatalog RELCAT;
    int ret =
        RELCAT.getTableEntry(*(args.joinArgs.buildTableName), buildTableEntry);

    // create bplus tree

    if (!bPlusTreeContainer->isIndexedForCoreNumber(args.joinArgs.coreNum)) {
      if (args.joinArgs.buildTableAttr->type == INTEGER) {
        BPlusTree<int *> *bPlusTree =
            new BPlusTree<int *>(args.joinArgs.buildTableAttr, buildTableEntry,
                                 args.joinArgs.coreNum);
        bPlusTree->buildTree();
        bPlusTreeContainer->setTreeRef((void *)bPlusTree,
                                       args.joinArgs.coreNum);
      }

      else if (args.joinArgs.buildTableAttr->type == STRING) {
        BPlusTree<char *> *bPlusTree =
            new BPlusTree<char *>(args.joinArgs.buildTableAttr, buildTableEntry,
                                  args.joinArgs.coreNum);
        bPlusTree->buildTree();
        bPlusTreeContainer->setTreeRef((void *)bPlusTree,
                                       args.joinArgs.coreNum);
      }
    }

    void *bPlusTree = bPlusTreeContainer->getTreeRef(args.joinArgs.coreNum);

    void *hit_tuple = nullptr;
    switch (args.joinArgs.probeTableAttr->type) {

    case INTEGER: {
      BPlusTree<int *> *bPlusTreeInt = (BPlusTree<int *> *)bPlusTree;
      // search the bplus tree

      Tuple tup;
      int *probe_input_tuple_val = (int *)tup.getTupleValue(
          args.joinArgs.probeTableAttr, probe_input_tuple);
      hit_tuple =
          bPlusTreeInt->search(((int *)probe_input_tuple), args.joinArgs.op);
      break;
    }

    case STRING: {
      BPlusTree<char *> *bPlusTreeString = (BPlusTree<char *> *)bPlusTree;
      Tuple tup;
      char *probe_input_tuple_val = (char *)tup.getTupleValue(
          args.joinArgs.probeTableAttr, probe_input_tuple);

      hit_tuple = bPlusTreeString->search((char *)(probe_input_tuple_val),
                                          args.joinArgs.op);
      break;
    }
    default: {
      break;
    }
    }

    // if hit tuple is not null then insert the tuple into the output tuple
    // stream
    if (hit_tuple != nullptr) {
      void *output_tuple = (void *)malloc(
          sizeof(char) * args.joinArgs.output_ts->getEntrySize());
      for (const auto &attr1 : args.joinArgs.output_ts->getAttributeList()) {
        bool hit = false;
        // if the attribute is the selected attribute then copy the value

        // the output tuple
        for (const auto &attr2 : probe_table_ts->getAttributeList()) {
          if (attr1.name == attr2.name) {
            hit = true;
            memcpy((char *)output_tuple + attr1.offset,
                   (char *)probe_input_tuple + attr2.offset, attr1.size);
          }
        }

        if (hit == false) {
          for (const auto &attr2 : buildTableEntry->getAttributes()) {
            if (attr1.name == attr2.name) {
              hit = true;
              memcpy((char *)output_tuple + attr1.offset,
                     (char *)hit_tuple + attr2.offset, attr1.size);
            }
          }
        }
      }
      args.joinArgs.output_ts->insert(output_tuple);
    }
  }
  return true;
}

void *linearSearch(LoopFnArgs args, void *probe_input_tuple,
                   ReadTupleStream *build_table_input_ts) {

  StaticVars staticVar;
  RelationCatalog RELCAT;
  RelationCatalogEntry *buildTableEntry = new RelationCatalogEntry();
  int ret =
      RELCAT.getTableEntry(*(args.joinArgs.buildTableName), buildTableEntry);

  while (true) {
    void *build_input_tuple = build_table_input_ts->yieldNext();
    if (build_input_tuple == NULL) {
      return nullptr;
    }

    Tuple tup;
    void *build_input_tuple_val =
        tup.getTupleValue(args.joinArgs.buildTableAttr, build_input_tuple);
    void *probe_input_tuple_val =
        tup.getTupleValue(args.joinArgs.probeTableAttr, probe_input_tuple);

    bool matched = Operator::match(
        build_input_tuple_val, probe_input_tuple_val, args.joinArgs.op,
        args.joinArgs.probeTableAttr->type, args.joinArgs.entrySize);

    if (matched) {
      return build_input_tuple;
    }
  }
  return nullptr;
}
