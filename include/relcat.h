// #ifndef RELCAT_H
// #define RELCAT_H
#pragma once
#include "attribute.h"
#include "morsel.h"
#include <constants.h>
#include <list>
#include <map>
#include "test.h"

class RelationCatalogEntry {
private:
  std::string tableName;
  std::list<Attribute> attributeList;
  // TODO: sixe of table

public:
  int num_records;
  int maxMorselSize;
  std::map<int, Morsel *> threadMap;
  std::map<size_t, Morsel *> numaNodeMap;

  RelationCatalogEntry();
  std::string getTableName();
  std::list<Attribute> getAttributes();
  std::list<Attribute> *getAttributesRef();

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
  static RelationCatalogEntry *getTableEntryRef(const std::string &tableName);
  static void deleteRelcat();

  static int appendToThreadMapMorsel(const std::string &tableName, int core,
                                     void *morselEntry);
};

// Global variable referencing the relcat object
// extern RelationCatalog RELCAT;
// #endif