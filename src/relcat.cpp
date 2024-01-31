#include "relcat.h"

std::string RelationCatalogEntry::getTableName() { return tableName; }

std::list<std::string> RelationCatalogEntry::getColNameList() {
  return colNameList;
}

std::list<int> RelationCatalogEntry::getColTypeList() { return colTypeList; }

std::string RelationCatalogEntry::setTableName(std::string name) {
  tableName = name;
}

int RelationCatalogEntry::setColNameList(std::list<std::string> nameList) {
  colNameList = nameList;
  return 0;
}

int RelationCatalogEntry::setColTypeList(std::list<int> typeList) {
  colTypeList = typeList;
  return 0;
}

int RelationCatalog::insertNewTable(std::string name,
                                    std::list<std::string> colNameList,
                                    std::list<int> colTypeList) {
  // create new relations cat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  // fill the object
  entry->setTableName(name);
  entry->setColNameList(colNameList);
  entry->setColTypeList(colTypeList);

  // append to catList

  catList.emplace_back(entry);
  return 0;
}

int RelationCatalog::getTableEntry(std::string tableName,
                                   RelationCatalogEntry *vesselPtr) {
  // search the catList

  for (auto iter = catList.begin(); iter != catList.end(); iter++) {
    if (tableName == iter->getTableName()) {
      vesselPtr->setTableName(iter->getTableName());
      vesselPtr->setColNameList(iter->getColNameList());
      vesselPtr->setColTypeList(iter->getColTypeList());
      return 0;
    }
  }
  return 1;
}
