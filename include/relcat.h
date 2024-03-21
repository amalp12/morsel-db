#ifndef RELCAT_H
#define RELCAT_H
#include "static.h"
#include <constants.h>
#include <iostream>
#include <map>
#include <list>
#include <list>
#include "attribute.h"
#include "morsel.h"


class RelationCatalogEntry {
private:
  std::string tableName;
  std::list<Attribute> attributeList;

public:
  std::map<int, Morsel *> threadMap;
  RelationCatalogEntry();
  std::string getTableName();
  std::list<Attribute> getAttributes();
  int setTableName(std::string name);
  int setAttributes(std::list<std::string> colNameList,
                    std::list<int> colTypeList);
  int setAttributes(std::list<Attribute> attributeList);
  void clearEntry();
};

class RelationCatalog {
private:
  static std::list<RelationCatalogEntry> catList;

public:
  static int insertNewTable(std::string name,
                                    std::list<std::string> colNameList,
                                    std::list<int> colTypeList);
  static int insertNewTable(std::string name,
                                    std::list<Attribute> attrs);
  static int getTableEntry(std::string tableName,
                           RelationCatalogEntry *vesselPtr);
  static void deleteRelcat();
                           

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