#include "relcat.h"
 
std::vector<RelationCatalogEntry> RelationCatalog::catList;

RelationCatalogEntry::RelationCatalogEntry() {
  tableName = "";
  attributeList.clear();
}

std::string RelationCatalogEntry::getTableName() { return tableName; }

std::vector<Attribute> RelationCatalogEntry::getAttributes() {
  return attributeList;
}


int RelationCatalogEntry::setTableName(std::string name) {
  tableName = name;
  return 0;
}




// get attribute size from the type

  int RelationCatalogEntry::setAttributes(std::vector<Attribute> attributeList){
    this->attributeList = attributeList;
    return 0;
  }


int RelationCatalogEntry::setAttributes(std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList) {
  for (size_t i = 0; i < colNameList.size(); i++) {
    Attribute attr;
    attr.name = colNameList[i];
    attr.type = colTypeList[i];
    attr.size = getAttributeSizeFromType(colTypeList[i]);
    if(i==0){
      attr.offset = 0;
    }else{
      attr.offset = attributeList[i-1].offset + attributeList[i-1].size;
    }
    attr.attributeIndex = i;
    attributeList.emplace_back(attr);
  }
  return 0;
}



int RelationCatalog::insertNewTable(std::string name,
                                    std::vector<Attribute> attrs) {
  // create new relations cat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  // fill the object
  entry->setTableName(name);
  entry->setAttributes(attrs);
  
  int entrySize = 0;
  for(auto iter = attrs.begin() ; iter != attrs.end() ; iter++)
  {
    entrySize += iter->size;
  }

  for(int coreNum = 0 ; coreNum < NUMBER_OF_CORES ; coreNum++)
  {
    entry->threadMap[coreNum] = new Morsel(MAX_MORSEL_SIZE , entrySize);
  }

  // append to catList
  catList.emplace_back(*entry);
  delete entry;

  return 0;
}

int RelationCatalog::insertNewTable(std::string name,
                                    std::vector<std::string> colNameList,
                                    std::vector<int> colTypeList) {
  // create new relations cat entry
  RelationCatalogEntry *entry = new RelationCatalogEntry();

  // fill the object
  entry->setTableName(name);
  entry->setAttributes(colNameList, colTypeList);  
  
  std::vector<Attribute> attributeList = entry->getAttributes();
  int entrySize = 0;
  for(auto iter = attributeList.begin() ; iter != attributeList.end() ; iter++)
  {
    entrySize += iter->size;
  }

  for(int coreNum = 0 ; coreNum < NUMBER_OF_CORES ; coreNum++)
  {
    entry->threadMap[coreNum] = new Morsel(MAX_MORSEL_SIZE , entrySize);
  }

  // append to catList
  catList.emplace_back(*entry);

  return 0;
}

int RelationCatalog::getTableEntry(std::string tableName,
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

int relcatInit(){


  // create a relation entry for the relation catalog itself

  std::vector<std::string> colNameList;
  std::vector<int> colTypeList;
  std::map<int, void* > threadMap;



  colNameList.emplace_back("table_name");
  colNameList.emplace_back("column_name_vector");
  colNameList.emplace_back("column_type_vector");
  colNameList.emplace_back("thread_to_morsel_map");

  // insert into RELCAT
  colTypeList.emplace_back(STRING);
  colTypeList.emplace_back(STRING);



  RelationCatalog RELCAT;
  RELCAT.insertNewTable("_relcat", colNameList, colTypeList);
  return 0;
}
