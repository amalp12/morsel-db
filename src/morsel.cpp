
#include "morsel.h"

Morsel::Morsel(int morselSize, int morselEntrySize) {
  subOperator = "";
  argList = {};
  next = NULL;
  size = morselSize;
  entrySize = morselEntrySize;
  // allocate memory
  startPtr = (void *)new char[morselSize];
  endPtr = startPtr + morselSize;
  currentPtr = startPtr;
}

std::string Morsel::getSubOperator() { return subOperator; }

std::list<std::string> Morsel::getArgList() { return argList; }

Morsel *Morsel::getNext() { return next; }

int Morsel::setSubOperator(std::string op) {
  subOperator = op;
  return 0;
}

int Morsel::setArgList(std::list<std::string> argumentsList) {
  argList = argumentsList;
  return 0;
}
int Morsel::setNext(Morsel *nextPtr) {
  next = nextPtr;
  return 0;
}

// memory management
void *Morsel::getStartPtr() { return startPtr; }

void *Morsel::getEndPtr() { return endPtr; }

void *Morsel::getCurrentPtr() { return currentPtr; }

int Morsel::setCurrentPtr(void *ptr) {
  currentPtr = ptr;
  return 0;
}

void *Morsel::getNthMorsel(int n) { return startPtr + n * entrySize; }

int Morsel::setNthMorsel(int n, void *ptr) {
  void *nthMorsel = getNthMorsel(n);
  std::copy(nthMorsel, ptr, entrySize);
  return 0;
}
