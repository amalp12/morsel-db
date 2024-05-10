#include "tuple.h"

// get tuple value from attribute
void *Tuple::getTupleValue(Attribute *attr, void *tuple) {
  void *val = (void *)((char *)tuple + attr->offset);
  return val;
}

// constructor

TupleStream::TupleStream(RelationCatalogEntry *relEntry, int coreNum) {
  this->morsel = relEntry->getMorselHead(coreNum);
  this->intitialMorsel = this->morsel;

  this->currentIndex = 0;
  this->entrySize = this->morsel->getEntrySize();
  this->morselStart = morsel->getStartPtr();

  // set attributes
  this->attributeList = relEntry->getAttributes();
}

TupleStream::TupleStream(Morsel *morsel, std::list<std::string> colNameList,
                         std::list<int> colTypeList) {
  this->intitialMorsel = morsel;
  this->morsel = morsel;
  this->currentIndex = 0;
  this->entrySize = 0;
  this->morselStart = morsel->getStartPtr();

  // set attributes
  this->setAttributes(colNameList, colTypeList);

  // calculate entry size
  for (auto iter = attributeList.begin(); iter != attributeList.end(); iter++) {
    this->entrySize += (*iter).size;
  }
}

void TupleStream::printStream() {

  // reset the tuple stream
  this->resetStream();
  Morsel *morselPtr = morsel;

  // get the attribute list
  const std::list<Attribute> attributeList = getAttributeList();
  // get entry size
  // int entrySize = 0;
  for (auto iter = attributeList.begin(); iter != attributeList.end(); iter++) {
    // entrySize += attributeList[i].size;
    // print the attribute name
    std::cout << (*iter).name << " ";
  }
  std::cout << std::endl;

  // get the number of entries filled
  int numEntriesFilled = morselPtr->getFilledNumberOfEntries();

  // get the morsel start
  // void *morselStart = m->getStartPtr();

  Tuple tup;
  int entriesCount = 0;
  int morselCount = 0;
  // iterate through all morsels
  while (morselPtr != NULL) {
    morselCount++;
    // for each morsel, iterate through all entries
    for (int entryNo = 0; entryNo < numEntriesFilled; entryNo++) {
      entriesCount++;
      void *entry = morselPtr->getNthMorselEntry(entryNo);
      for (auto iter = attributeList.begin(); iter != attributeList.end();
           iter++) {
        auto iterVal = *iter;
        void *value = tup.getTupleValue(&iterVal, entry);
        if (iter->type == INTEGER) {
          std::cout << *(int *)value << " ";
        } else if (iter->type == FLOAT) {
          std::cout << *(float *)value << " ";
        } else if (iter->type == STRING) {
          std::cout << (char *)value << " ";
        }
      }
      std::cout << std::endl;
    }

    morselPtr = morselPtr->getNext();
    if (morselPtr != nullptr) {
      numEntriesFilled = morselPtr->getFilledNumberOfEntries();
    }
  }
  // print counts
  std::cout << "Morsel Count : " << morselCount << "\n";
  std::cout << "Entries Count : " << entriesCount << "\n";
}
void TupleStream::resetStream() {
  this->morsel = this->intitialMorsel;
  this->currentIndex = 0;
  this->morselStart = morsel->getStartPtr();
}

void *ReadTupleStream::yieldNext() {

  // if current morsel is exhausted, move to next morsel
  if (currentIndex >= morsel->getFilledNumberOfEntries()) {
    morsel = morsel->getNext();
    // if no more morsels or current morsel is null, return NULL
    if (morsel == NULL) {
      return NULL;
    }
    morselStart = morsel->getStartPtr();
    currentIndex = 0;
  }

  // if no more morsels or current morsel is null, return NULL
  if (morsel == NULL) {
    return NULL;
  }
  // perform artihmetic to get the next tuple
  void *ret = (void *)((char *)morselStart + (this->currentIndex * entrySize));

  this->currentIndex++;
  return ret;
}

int WriteTupleStream::insert(void *tuple) {

  // get current morsel and check if its exhasuted
  // if yes
  // create new morsel assign next and reassgin morsel to new morsel
  // insert into morsel

  // return sucess

  // if current morsel is exhausted, move to next morsel
  morsel->insertEntry(tuple);

  if (this->currentIndex >= morsel->getTotalNumberOfEntries()) {
    morsel = morsel->getNext();
    morselStart = morsel->getStartPtr();
    currentIndex = 0;
  }

  this->currentIndex++;
  return 1;
}

std::list<Attribute> TupleStream::getAttributeList() { return attributeList; }

int TupleStream::setAttributes(std::list<std::string> colNameList,
                               std::list<int> colTypeList) {

  auto iter_colNameList = colNameList.begin();
  auto iter_colTypeList = colTypeList.begin();
  auto iter = attributeList.begin();
  int index = 0;
  while (iter_colNameList != colNameList.end() &&
         iter_colTypeList != colTypeList.end()) {
    Attribute attr;
    attr.name = *iter_colNameList;
    attr.type = *iter_colTypeList;
    attr.size = getAttributeSizeFromType(*iter_colTypeList);
    if (index == 0) {
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

int TupleStream::getEntrySize() { return this->entrySize; }

void TupleStream::writeStream(std::string filename) {

  std::ofstream file;
  // open file
  file.open(filename);
  // reset the tuple stream
  this->resetStream();
  Morsel *m = morsel;

  // get the attribute list
  std::list<Attribute> attributeList = getAttributeList();
  // get entry size
  // int entrySize = 0;
  for (auto iter = attributeList.begin(); iter != attributeList.end(); iter++) {
    // entrySize += attributeList[i].size;
    // append  attribute name
    file << iter->name << " ";
  }
  file << '\n';

  // get the number of entries
  int numEntriesFilled = m->getFilledNumberOfEntries();

  // get the morsel start
  // void *morselStart = m->getStartPtr();

  Tuple tup;
  // iterate through all morsels
  while (m != NULL) {
    // for each morsel, iterate through all entries
    for (int entryNo = 0; entryNo < numEntriesFilled; entryNo++) {
      void *entry = m->getNthMorselEntry(entryNo);
      for (auto iter = attributeList.begin(); iter != attributeList.end();
           iter++) {
        Attribute attr = *iter;
        void *value = tup.getTupleValue(&attr, entry);
        if (attr.type == INTEGER) {
          file << *(int *)value << " ";
        } else if (attr.type == FLOAT) {
          file << *(float *)value << " ";
        } else if (attr.type == STRING) {
          file << (char *)value << " ";
        }
        // else append ERROR
        else {
          file << "ERROR!FIXME"
               << " ";
        }
      }
      file << '\n';
    }

    m = m->getNext();
    if (m != nullptr) {
      numEntriesFilled = m->getFilledNumberOfEntries();
    }
  }
  file.flush();
  file.close();
}
