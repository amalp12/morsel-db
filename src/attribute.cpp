#include "attribute.h"
#include "constants.h"
#include "relcat.h"
#include "static.h"

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

bool BPlusTreeContainer::isIndexedForCoreNumber(int coreNumber) {
  if (this->bPlusTreeRef && this->bPlusTreeRef[coreNumber - 1] != NULL) {
    return true;
  }
  return false;
}

BPlusTreeContainer::BPlusTreeContainer(std::string tableName) {

  this->size = StaticVars::getNumberOfCores();

  // create bPlusTreeRef array of size equal to number of cores and initialize
  // to NULL
  this->bPlusTreeRef = new void *[this->size];
  for (int i = 0; i < this->size; i++) {
    this->bPlusTreeRef[i] = NULL;
  }
}

int BPlusTreeContainer::setTreeRef(void *treeRef, int coreNumber) {

  if (coreNumber <= 0 || coreNumber > this->size || treeRef == NULL ||
      this->bPlusTreeRef[coreNumber - 1] != NULL) {
    return FAILURE;
  }
  this->bPlusTreeRef[coreNumber - 1] = treeRef;

  return SUCCESS;
}

void *BPlusTreeContainer::getTreeRef(int coreNumber) {
  return this->bPlusTreeRef[coreNumber - 1];
}