#pragma once
#include <vector>
#include <memory>
#include <string>
#include "attribute.h"
#include <vector>
#include <cstring>

class Morsel {

private:
  Morsel *next;
  int size;
  int entrySize;
  void *startPtr;
  int nextFreeIndex;
  int filledEntries;
  int setNthMorselEntry(int n, void *ptr);
  int setNthMorselEntry(int n, void *ptr , std::vector<Attribute> attributeList);
  int setNext(Morsel *nextPtr);

public:
  void *getNthMorselEntry(int n);
  void *getStartPtr();
  Morsel(int morselSize, int morselEntrySize);
  Morsel *getNext();
  int insertEntry(void *entry);
  int insertEntry(void *entry , std::vector<Attribute> attributeList);

  int getTotalSize();
  int getEntrySize();
  int getTotalNumberOfEntries();
  int getFilledNumberOfEntries();
};


class MorselContainer {
  private:
  std::string subOperator;
  std::vector<std::string> argList;
  Morsel *morsel;
  public:
  MorselContainer(Morsel *morsel) { this->morsel = morsel; }
  std::string getSubOperator();
  std::vector<std::string> getArgList();
  int setSubOperator(std::string op);
  int setArgList(std::vector<std::string> argumentsList);
};

