// #ifndef RELCAT_H
// #define RELCAT_H
#pragma once
#include "attribute.h"
#include "morsel.h"
#include "static.h"
#include <constants.h>
#include <list>
#include <map>

class RelationCatalogEntry {
private:
  std::string tableName;
  std::list<Attribute> attributeList;
  // TODO: sixe of table

public:
  int num_records;
  std::map<int, Morsel *> threadMap;
  RelationCatalogEntry();
  std::string getTableName();
  std::list<Attribute> getAttributes();
  int setTableName(const std::string &name);
  int setAttributes(const std::list<std::string> &colNameList,
                    const std::list<int> &colTypeList);
  int setAttributes(const std::list<Attribute> &attributeList);
  void clearEntry();
};

class RelationCatalog {
private:
  static std::list<RelationCatalogEntry> catList;

public:
  static int insertNewTable(const std::string &name,
                            const std::list<std::string> &colNameList,
                            const std::list<int> &colTypeList);
  static int insertNewTable(const std::string &name,
                            const std::list<Attribute> &attrs);
  static int getTableEntry(const std::string &tableName,
                           RelationCatalogEntry *vesselPtr);
  static void deleteRelcat();

  static int appendToThreadMapMorsel(const std::string &tableName, int core,
                                     void *morselEntry) {

    for (auto iter = catList.begin(); iter != catList.end(); iter++) {
      if (tableName == iter->getTableName()) {
        iter->threadMap[core]->insertEntry(morselEntry);
      }
    }
    return 0;
  }
};

// Global variable referencing the relcat object
// extern RelationCatalog RELCAT;
// #endif