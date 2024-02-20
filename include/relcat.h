#ifndef RELCAT_H
#define RELCAT_H
#include <constants.h>
#include <iostream>
#include <map>
#include <vector>
#include "attribute.h"
#include "morsel.h"


class RelationCatalogEntry {
private:
  std::string tableName;
  std::vector<Attribute> attributeList;

public:
  std::map<int, Morsel *> threadMap;
  RelationCatalogEntry();
  std::string getTableName();
  std::vector<Attribute> getAttributes();
  int setTableName(std::string name);
  int setAttributes(std::vector<std::string> colNameList,
                    std::vector<int> colTypeList);
  int setAttributes(std::vector<Attribute> attributeList);
};

class RelationCatalog {
private:
  static std::vector<RelationCatalogEntry> catList;

public:
  static int insertNewTable(std::string name,
                                    std::vector<std::string> colNameList,
                                    std::vector<int> colTypeList);
  static int insertNewTable(std::string name,
                                    std::vector<Attribute> attrs);
  static int getTableEntry(std::string tableName,
                           RelationCatalogEntry *vesselPtr);

  static int appendToThreadMapMorsel(std::string tableName , int core , void * morselEntry){

   for (auto iter = catList.begin(); iter != catList.end(); iter++) {
    if (tableName == iter->getTableName()) {
      iter->threadMap[core]->insertEntry(morselEntry);
    }
   }
    return 0;
  }
};

// Global variable referencing the relcat object
//extern RelationCatalog RELCAT;
#endif