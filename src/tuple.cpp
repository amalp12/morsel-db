#include "tuple.h"


// get tuple value from attribute
void* Tuple::getTupleValue(Attribute * attr, void *tuple) {
  void * val  = (void *)((char *)tuple + attr->offset);
  return val;
}


// constructor

TupleStream::TupleStream(RelationCatalogEntry *relEntry, int coreNum) {
  this->morsel = relEntry->threadMap[coreNum];
  this-> intitialMorsel=this->morsel;

  this->currentIndex = 0;
  this->entrySize = this->morsel->getEntrySize();
  this->morselStart = morsel->getStartPtr();
  

  // set attributes
  this->attributeList = relEntry->getAttributes();

  
}

TupleStream::TupleStream(Morsel *morsel, std::vector<std::string> colNameList,
                                       std::vector<int> colTypeList) {
  this->intitialMorsel=morsel;
  this->morsel = morsel;
  this->currentIndex = 0;
  this->entrySize = 0;
  this->morselStart = morsel->getStartPtr();

  // set attributes
  this->setAttributes(colNameList, colTypeList);


  // calculate entry size
  for (size_t i = 0; i < attributeList.size(); i++) {
    this->entrySize += attributeList[i].size;
  }
}

void TupleStream::printStream() {
  Morsel *m = morsel;

  // get the attribute list
  std::vector<Attribute> attributeList = getAttributeList();
  // get entry size
  // int entrySize = 0;
  for (size_t i = 0; i < attributeList.size(); i++) {
    // entrySize += attributeList[i].size;
    // print the attribute name
    std::cout << attributeList[i].name << " ";
  }
  std::cout << std::endl;

  // get the number of entries filled
  int numEntriesFilled = m->getFilledNumberOfEntries();

  // get the morsel start
  // void *morselStart = m->getStartPtr();

  Tuple tup;
  // iterate through all morsels
  while (m != NULL) {
    // for each morsel, iterate through all entries
    for(int entryNo=0;entryNo<numEntriesFilled;entryNo++){
      void *entry = m->getNthMorselEntry(entryNo);
      for (size_t i = 0; i < attributeList.size(); i++) {
        Attribute attr = attributeList[i];
        void *value = tup.getTupleValue(&attr, entry);
        if (attr.type == INTEGER) {
          std::cout << *(int *)value << " ";
        } else if (attr.type == FLOAT) {
          std::cout << *(float *)value << " ";
        } else if (attr.type == STRING) {
          std::cout << (char *)value << " ";
        }
      }
      std::cout << std::endl;
    }

    m = m->getNext();
  }
}
void TupleStream::resetStream(){
  this->morsel=this->intitialMorsel;
  this->currentIndex = 0;
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


int WriteTupleStream::insert(void * tuple) {

  // get current morsel and check if its exhasuted
    // if yes
      // create new morsel assign next and reassgin morsel to new morsel
    // insert into morsel

  // return sucess

  // if current morsel is exhausted, move to next morsel
  morsel->insertEntry(tuple);

  if (currentIndex >= morsel->getTotalNumberOfEntries()) {   
    morsel = morsel->getNext();
    morselStart = morsel->getStartPtr();
    currentIndex = 0;
  }

  this->currentIndex++;
  return 1;   
}



std::vector<Attribute> TupleStream::getAttributeList() {
  return attributeList;
}

int TupleStream::setAttributes(std::vector<std::string> colNameList,
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

int TupleStream::getEntrySize(){
  return this->entrySize;
}

void TupleStream::writeStream(std::string filename)
{
  
  std::ofstream file;
  // open file
  file.open(filename);
  Morsel *m = morsel;

  // get the attribute list
  std::vector<Attribute> attributeList = getAttributeList();
  // get entry size
  // int entrySize = 0;
  for (size_t i = 0; i < attributeList.size(); i++) {
    // entrySize += attributeList[i].size;
    // append  attribute name
    file << attributeList[i].name << " ";
  }
  file << std::endl;

  // get the number of entries
  int numEntriesFilled = m->getTotalNumberOfEntries();

  // get the morsel start
  // void *morselStart = m->getStartPtr();

  Tuple tup;
  // iterate through all morsels
  while (m != NULL) {
    // for each morsel, iterate through all entries
    for(int entryNo=0;entryNo<numEntriesFilled;entryNo++){
      void *entry = m->getNthMorselEntry(entryNo);
      for (size_t i = 0; i < attributeList.size(); i++) {
        Attribute attr = attributeList[i];
        void *value = tup.getTupleValue(&attr, entry);
        if (attr.type == INTEGER) {
          file << *(int *)value << " ";
        } else if (attr.type == FLOAT) {
          file << *(float *)value << " ";
        } else if (attr.type == STRING) {
          file << (char *)value << " ";
        }
        // else append ERROR
        else{
          file << "ERROR!FIXME" << " ";
        
        }
      }
      file << std::endl;
    }

    m = m->getNext();
  }
  file.close();
}
