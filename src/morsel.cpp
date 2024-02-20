
#include "morsel.h"

Morsel::Morsel(int morselSize, int morselEntrySize) {
  next = NULL;
  size = morselSize;
  entrySize = morselEntrySize;
  // allocate memorygetTableName
  startPtr = (void *)new char[morselSize];
  nextFreeIndex = 0;
}

std::string MorselContainer::getSubOperator() { return subOperator; }

std::vector<std::string> MorselContainer::getArgList() { return argList; }

Morsel *Morsel::getNext() { return next; }

int MorselContainer::setSubOperator(std::string op) {
  subOperator = op;
  return 0;
}

int MorselContainer::setArgList(std::vector<std::string> argumentsList) {
  argList = argumentsList;
  return 0;
}
int Morsel::setNext(Morsel *nextPtr) {
  next = nextPtr;
  return 0;
}

// memory management
void *Morsel::getStartPtr() { return startPtr; }




void *Morsel::getNthMorselEntry(int n) {
  int entryWithMetadataSize = entrySize;
  return (void *)((char *)startPtr +( n * entryWithMetadataSize));
}

int Morsel::setNthMorselEntry(int n, void *ptr) {
  void *nthMorsel = getNthMorselEntry(n);
  memcpy(nthMorsel, ptr , entrySize);
  //copy 
  // std::copy((char *)ptr, (char *)nthMorsel, entrySize);
  return 0;
}

int Morsel::setNthMorselEntry(int n, void *ptr, std::vector<Attribute> attributeList) {
  void *nthMorsel = getNthMorselEntry(n);
  int offset = 0;
  for (size_t i = 0; i < attributeList.size(); i++) {
    Attribute attr = attributeList[(int)i];
    memcpy((char *)nthMorsel + offset , (char *)ptr + attr.offset, attr.size);
    offset += attr.size;
  }
  return 0;
}


int Morsel::getTotalSize() { return size; }
int Morsel::getEntrySize() { return entrySize; }
int Morsel::getTotalNumberOfEntries() { return size / entrySize; }

int Morsel::insertEntry(void *entry) {
  if (this->nextFreeIndex >= getTotalNumberOfEntries()) {
    if (next == NULL) {
      // create new morsel
      next = new Morsel(size, entrySize);
    }
    return next->insertEntry(entry);
  }
  setNthMorselEntry(this->nextFreeIndex, entry);
  // increment next free index
  this->nextFreeIndex++;
  // increment filled entries
  this->filledEntries++;

  return 0;
}


int Morsel::insertEntry(void *entry , std::vector<Attribute> attributeList){
  if (this->nextFreeIndex >= getTotalNumberOfEntries()) {
    if (next == NULL) {
      // create new morsel
      next = new Morsel(size, entrySize);
    }
    return next->insertEntry(entry , attributeList);
  }
  
  setNthMorselEntry(this->nextFreeIndex, entry , attributeList);
  this->nextFreeIndex++;
  // increment filled entries
  this->filledEntries++;
  return 0;
}

int Morsel::getFilledNumberOfEntries(){
  return filledEntries;
}
