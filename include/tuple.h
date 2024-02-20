#pragma once
#include "constants.h"
#include "morsel.h"
#include <string>
#include <vector>
#include "attribute.h"
#include "relcat.h"
#include <fstream>

//static class for tuple operations
class Tuple{
public:
     void * getTupleValue(Attribute *attr, void *tuple);
};

class TupleStream {
protected:
  Morsel * intitialMorsel;
  Morsel *morsel;
  void *morselStart;
  std::vector<Attribute> attributeList;
  int entrySize; // in bytes
  int currentIndex;

public:
  TupleStream(Morsel *morsel, 
            std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList);
  TupleStream(RelationCatalogEntry *relEntry, int coreNum);

  void printStream();
  void writeStream(std::string filename);
  void resetStream();

  std::vector<Attribute> getAttributeList();
  int setAttributes(std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList);

  int getEntrySize();
};


class ReadTupleStream: public TupleStream{
  public:
    ReadTupleStream(Morsel *morsel, 
            std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList) : TupleStream(morsel, colNameList, colTypeList){
      
    }
    ReadTupleStream(RelationCatalogEntry *relEntry, int coreNum) :  TupleStream(relEntry, coreNum){

    }
    void *yieldNext();

};


class WriteTupleStream: public TupleStream{
  public:
    WriteTupleStream(Morsel *morsel, 
            std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList) : TupleStream(morsel, colNameList, colTypeList){
      
    }
    WriteTupleStream(RelationCatalogEntry *relEntry, int coreNum) :  TupleStream(relEntry, coreNum){
      
    }
  int insert(void * tuple);


};