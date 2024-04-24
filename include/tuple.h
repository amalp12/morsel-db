#pragma once
#include "attribute.h"
#include "constants.h"
#include "morsel.h"
#include "relcat.h"
#include <fstream>
#include <iostream>
#include <list>
#include <string>

// static class for tuple operations
class Tuple {
public:
  void *getTupleValue(Attribute *attr, void *tuple);
};

class TupleStream {
protected:
  Morsel *intitialMorsel;
  Morsel *morsel;
  void *morselStart;
  std::list<Attribute> attributeList;
  int entrySize; // in bytes
  int currentIndex;

public:
  TupleStream(Morsel *morsel, std::list<std::string> colNameList,
              std::list<int> colTypeList);
  TupleStream(RelationCatalogEntry *relEntry, int coreNum);

  void printStream();
  void writeStream(std::string filename);
  void resetStream();

  std::list<Attribute> getAttributeList();
  int setAttributes(std::list<std::string> colNameList,
                    std::list<int> colTypeList);

  int getEntrySize();
};

class ReadTupleStream : public TupleStream {
public:
  ReadTupleStream(Morsel *morsel, std::list<std::string> colNameList,
                  std::list<int> colTypeList)
      : TupleStream(morsel, colNameList, colTypeList) {}
  ReadTupleStream(RelationCatalogEntry *relEntry, int coreNum)
      : TupleStream(relEntry, coreNum) {}
  void *yieldNext();
};

class WriteTupleStream : public TupleStream {
public:
  WriteTupleStream(Morsel *morsel, std::list<std::string> colNameList,
                   std::list<int> colTypeList)
      : TupleStream(morsel, colNameList, colTypeList) {}
  WriteTupleStream(RelationCatalogEntry *relEntry, int coreNum)
      : TupleStream(relEntry, coreNum) {}
  int insert(void *tuple);
};