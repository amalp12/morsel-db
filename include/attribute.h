#pragma once
#include <string>

class BPlusTreeContainer {
private:
  void **bPlusTreeRef = nullptr;
  int size;

public:
  BPlusTreeContainer(std::string tableName);
  int setTreeRef(void *treeRef, int coreNumber);
  void *getTreeRef(int coreNumber);
  bool isIndexedForCoreNumber(int coreNumber);
};

// TODO : initialize size of vector/list holding bplus tree refs of different
// cores
struct Attribute {
  std::string name;
  int type;
  int offset;
  int size;
  int attributeIndex;
  bool isIndexed = false;
  BPlusTreeContainer *bPlusTreeContainer = nullptr;
};

int getAttributeSizeFromType(int type);
