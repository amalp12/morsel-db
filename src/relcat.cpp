#include "relcat.h"

std::list<RelationCatalogEntry> RelationCatalog::catList;

RelationCatalogEntry::RelationCatalogEntry() {
  tableName = "";
  attributeList.clear();
}

std::string RelationCatalogEntry::getTableName() { return tableName; }

std::list<Attribute> RelationCatalogEntry::getAttributes() {
  return attributeList;
}

int RelationCatalogEntry::setTableName(const std::string &name) {
  tableName = name;
  return 0;
}

// get attribute size from the type

int RelationCatalogEntry::setAttributes(
    const std::list<Attribute> &attributeList) {
  this->attributeList = attributeList;
  return 0;
}

int RelationCatalogEntry::setAttributes(
    const std::list<std::string> &colNameList,
    const std::list<int> &colTypeList) {
  auto iter = attributeList.begin();
  auto iter_colNameList = colNameList.begin();
  auto iter_colTypeList = colTypeList.begin();
  int index = 0;

  while (iter_colNameList != colNameList.end() &&
         iter_colTypeList != colTypeList.end()) {
    Attribute attr;
    attr.name = *iter_colNameList;
    attr.type = *iter_colTypeList;
    attr.size = getAttributeSizeFromType(*iter_colTypeList);
    attr.bPlusTreeContainer = nullptr;
    if (iter_colNameList == colNameList.begin()) {
      attr.offset = 0;
    } else {
      attr.offset = attributeList.back().offset + attributeList.back().size;
    }
    attr.attributeIndex = index;
    attributeList.emplace_back(attr);
    index++;
    iter++;
    iter_colNameList++;
    iter_colTypeList++;
  }
  return 0;
}

int RelationCatalog::insertNewTable(const std::string &name,
                                    const std::list<Attribute> &attrs) {
  // staticvars
  StaticVars staticVars;
  // create new relations cat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  // fill the object
  entry->setTableName(name);
  entry->setAttributes(attrs);

  int entrySize = 0;
  for (auto iter = attrs.begin(); iter != attrs.end(); iter++) {
    entrySize += iter->size;
  }

  for (int coreNum = 1; coreNum <= staticVars.getNumberOfCores(); coreNum++) {
    entry->threadMap[coreNum] =
        new Morsel(staticVars.getMaxMorselSize(), entrySize);
  }

  // append to catList
  catList.emplace_back(*entry);
  delete entry;

  return 0;
}

int RelationCatalog::insertNewTable(const std::string &name,
                                    const std::list<std::string> &colNameList,
                                    const std::list<int> &colTypeList) {
  // staticvars
  StaticVars staticVars;
  // create new relations cat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  // fill the object
  entry->setTableName(name);
  entry->setAttributes(colNameList, colTypeList);

  std::list<Attribute> attributeList = entry->getAttributes();
  int entrySize = 0;
  for (auto iter = attributeList.begin(); iter != attributeList.end(); iter++) {
    entrySize += iter->size;
  }

  for (int coreNum = 1; coreNum <= staticVars.getNumberOfCores(); coreNum++) {
    entry->threadMap[coreNum] =
        new Morsel(staticVars.getMaxMorselSize(), entrySize);
  }

  // append to catList
  catList.emplace_back(*entry);
  delete entry;

  return 0;
}

int RelationCatalog::getTableEntry(const std::string &tableName,
                                   RelationCatalogEntry *vesselPtr) {
  // search the catList

  for (auto iter = catList.begin(); iter != catList.end(); iter++) {
    if (tableName == iter->getTableName()) {
      vesselPtr->setTableName(iter->getTableName());
      vesselPtr->setAttributes(iter->getAttributes());
      vesselPtr->threadMap = iter->threadMap;
      return 0;
    }
  }
  return 1;
}

void RelationCatalogEntry::clearEntry() {
  tableName.clear();
  // for each entry in attirbuteList
  for (auto &attr : attributeList) {
    attr.name.clear();
  }
  // clear attibute list
  attributeList.clear();

  // for each entry in threadmap
  for (auto &thread : threadMap) {
    // deallocate all morsels
    Morsel *ptr = thread.second;
    while (ptr) {
      Morsel *temp = ptr;
      // free
      free(temp->getStartPtr());
      ptr = ptr->getNext();
      delete temp;
    }
  }
  // clear threadmap
  threadMap.clear();
}
void RelationCatalog::deleteRelcat() {

  // for each entry in catList
  for (auto &catEntry : catList) {

    // clear entry
    catEntry.clearEntry();
  }
  // clear catList
  catList.clear();
}

int relcatInit() {

  // create a relation entry for the relation catalog itself

  std::list<std::string> colNameList;
  std::list<int> colTypeList;
  std::map<int, void *> threadMap;

  colNameList.emplace_back("table_name");
  colNameList.emplace_back("column_name_list");
  colNameList.emplace_back("column_type_list");
  colNameList.emplace_back("thread_to_morsel_map");

  // insert into RELCAT
  colTypeList.emplace_back(STRING);
  colTypeList.emplace_back(STRING);

  RelationCatalog RELCAT;
  RELCAT.insertNewTable("_relcat", colNameList, colTypeList);
  return 0;
}
