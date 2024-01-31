#include <iostream>
#include <list>
#include <map>
#include <string>

class RelationCatalogEntry {
private:
  std::string tableName;
  std::list<std::string> colNameList;
  std::list<int> colTypeList;

public:
  std::string getTableName();
  std::list<std::string> getColNameList();
  std::list<int> getColTypeList();
  std::string setTableName(std::string name);
  int setColNameList(std::list<std::string> nameList);
  int setColTypeList(std::list<int> typeList);
};

class RelationCatalog {
private:
  std::list<RelationCatalogEntry> catList;

public:
  int insertNewTable(std::string name, std::list<std::string> colNameList,
                     std::list<int> colTypeList);
  int getTableEntry(std::string tableName, RelationCatalogEntry *vesselPtr);
};
