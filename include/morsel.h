#pragma once
#include "attribute.h"
#include <cstring>
#include <list>
#include <string>

class Morsel {

private:
  Morsel *next;
  int size;
  int entrySize;
  void *startPtr;
  int nextFreeIndex;
  int filledEntries;
  int setNthMorselEntry(int n, void *ptr);
  int setNthMorselEntry(int n, void *ptr, std::list<Attribute> attributeList);
  int setNext(Morsel *nextPtr);

public:
  void *getNthMorselEntry(int n);
  void *getStartPtr();
  Morsel(int morselSize, int morselEntrySize);
  Morsel *getNext();
  int insertEntry(void *entry);
  int insertEntry(void *entry, std::list<Attribute> attributeList);

  int getTotalSize();
  int getEntrySize();
  int getTotalNumberOfEntries();
  int getFilledNumberOfEntries();
};

class MorselContainer {
private:
  std::string subOperator;
  std::list<std::string> argList;
  Morsel *morsel;

public:
  MorselContainer(Morsel *morsel) { this->morsel = morsel; }
  std::string getSubOperator();
  std::list<std::string> getArgList();
  int setSubOperator(std::string op);
  int setArgList(std::list<std::string> argumentsList);
};
