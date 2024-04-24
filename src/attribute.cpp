#include "attribute.h"
#include "relcat.h"

int getAttributeSizeFromType(int type) {
  switch (type) {
  case STRING:
    return STRING_SIZE;
  case INTEGER:
    return INTEGER_SIZE;
  case FLOAT:
    return FLOAT_SIZE;
  default:
    return 0;
  }
}

BPlusTreeContainer::BPlusTreeContainer(std::string tableName) {
  // declate relcat object
  RelationCatalog relCat;

  StaticVars staticVars;

  this->size = staticVars.getNumberOfCores();

  // create bPlusTreeRef array of size equal to number of cores and initialize
  // to NULL
  this->bPlusTreeRef = new void *[this->size];
  for (int i = 0; i < this->size; i++) {
    this->bPlusTreeRef[i] = NULL;
  }

  // get relcat entry
  RelationCatalogEntry relcat_entry;
  relCat.getTableEntry(tableName, &relcat_entry);
}

int BPlusTreeContainer::insertTreeRef(void *treeRef) {
  // bPlusTreeRef.push_back(treeRef);

  return SUCCESS;
}
