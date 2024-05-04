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
  for (const auto output_attr : output_ts->getAttributeList()) {
    // if the attribute is the selected attribute then copy the value to the
    // output tuple
    for (const auto input_attr : input_ts->getAttributeList()) {
      if (input_attr.name == output_attr.name) {
        memcpy((char *)output_tuple + output_attr.offset,
               (char *)input_tuple + input_attr.offset, input_attr.size);
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
  //  ??
  void *probe_input_tuple = probe_table_ts->yieldNext();
  if (probe_input_tuple == NULL) {
    return false;
  }

  RelationCatalogEntry *buildTableEntry =
      RelationCatalog::getTableEntryRef(*(args.joinArgs.buildTableName));

  // if the table is not found
  if (not buildTableEntry) {
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
        for (const auto &output_attr :
             args.joinArgs.output_ts->getAttributeList()) {
          bool hit = false;
          // if the attribute is the selected attribute then copy the value

          // the output tuple
          for (const auto &probe_attr : probe_table_ts->getAttributeList()) {
            if (probe_attr.name == output_attr.name) {
              hit = true;
              memcpy((char *)output_tuple + output_attr.offset,
                     (char *)probe_input_tuple + probe_attr.offset,
                     probe_attr.size);
            }
          }

          if (hit == false) {
            for (const auto &build_attr :
                 build_table_input_ts->getAttributeList()) {
              if (build_attr.name == output_attr.name) {
                hit = true;
                memcpy((char *)output_tuple + output_attr.offset,
                       (char *)build_input_tuple + build_attr.offset,
                       build_attr.size);
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

    RelationCatalogEntry *buildTableEntry =
        RelationCatalog::getTableEntryRef(*(args.joinArgs.buildTableName));

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
      // error
      std::cout << "Error: Invalid type\n";
      exit(1);
      break;
    }
    }

    // if hit tuple is not null then insert the tuple into the output tuple
    // stream
    if (hit_tuple != nullptr) {

      // std::cout << "---Open---\n";
      // std::cout << "---Hit Tuple---\n";
      // // print hit tuple
      // for (const auto &attr : buildTableEntry->getAttributes()) {
      //   std::cout << "Attribute: " << attr.name << " Value: ";
      //   if (attr.type == INTEGER) {
      //     std::cout << *((int *)((char *)hit_tuple + attr.offset)) << "\n";

      //   } else if (attr.type == STRING) {
      //     std::cout << ((char *)hit_tuple + attr.offset) << "\n";
      //   }
      // }

      // for (const auto &attr : probe_table_ts->getAttributeList()) {
      //   std::cout << "Attribute: " << attr.name << " Value: ";
      //   if (attr.type == INTEGER) {
      //     std::cout << *((int *)((char *)hit_tuple + attr.offset)) << "\n";

      //   } else if (attr.type == STRING) {
      //     std::cout << ((char *)hit_tuple + attr.offset) << "\n";
      //   }
      // }
      // std::cout << "------------------\n";

      void *output_tuple = (void *)malloc(
          sizeof(char) * args.joinArgs.output_ts->getEntrySize());
      for (const auto &output_table_attr :
           args.joinArgs.output_ts->getAttributeList()) {
        bool hit_attr = false;
        // if the attribute is the selected attribute then copy the value

        // the output tuple
        for (const auto &probe_table_attr :
             probe_table_ts->getAttributeList()) {
          if (output_table_attr.name == probe_table_attr.name) {
            hit_attr = true;
            memcpy((char *)output_tuple + output_table_attr.offset,
                   (char *)probe_input_tuple + probe_table_attr.offset,
                   output_table_attr.size);
            break;
          }
        }

        if (hit_attr) {
          continue;
        }
        for (const auto &build_table_attr : buildTableEntry->getAttributes()) {
          if (output_table_attr.name == build_table_attr.name) {
            memcpy((char *)output_tuple + output_table_attr.offset,
                   (char *)hit_tuple + build_table_attr.offset,
                   output_table_attr.size);
            break;
          }
        }
      }

      // print ouput tuple
      // std::cout << "---Output Tuple---\n";
      // for (const auto &attr : args.joinArgs.output_ts->getAttributeList()) {
      //   std::cout << "Attribute: " << attr.name << " Value: ";
      //   if (attr.type == INTEGER) {
      //     if (attr.name == "Column_1_Table_A") {
      //       if (*((int *)((char *)output_tuple + attr.offset)) > 265000000) {
      //         std::cout << *((int *)((char *)output_tuple + attr.offset))
      //                   << "\n";
      //       }
      //     }
      //     std::cout << *((int *)((char *)output_tuple + attr.offset)) <<
      //     "\n";

      //   } else if (attr.type == STRING) {
      //     std::cout << ((char *)output_tuple + attr.offset) << "\n";
      //   }
      // }
      // std::cout << "------------------\n";
      // std::cout << "---Close---\n";

      args.joinArgs.output_ts->insert(output_tuple);
      // print the output tuple stream
      // std::cout << "---Output Tuple---\n";
      // args.joinArgs.output_ts->printStream();
      // std::cout << "------------------\n";
    }
  }
  return true;
}

void *linearSearch(LoopFnArgs args, void *probe_input_tuple,
                   ReadTupleStream *build_table_input_ts) {

  StaticVars staticVar;
  RelationCatalogEntry *buildTableEntry =
      RelationCatalog::getTableEntryRef(*(args.joinArgs.buildTableName));

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
