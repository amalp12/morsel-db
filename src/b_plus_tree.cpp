#include "b_plus_tree.h"
#include "constants.h"

// constructor
template <typename T>
BPlusTree<T>::BPlusTree(Attribute *attr, RelationCatalogEntry entry, int core) {
  root = new BPlusTree_LeafNode<T>(LEAF_NODE);
  // build
  attribute = attr;
  relcat_entry = entry;
  coreNumber = core;
}

// destructor
template <typename T> BPlusTree<T>::~BPlusTree() { destroy(); }

BPlusTree_Node *BPlusTree_Node::getParent() { return this->parent; }
void BPlusTree_Node::setParent(BPlusTree_Node *parent) {
  this->parent = parent;
}

BPlusTree_Node::BPlusTree_Node(int type) {
  this->parent = NULL;
  this->node_type = type;
  this->number_of_filled_units = 0;
}
BPlusTree_Node::BPlusTree_Node(int type, BPlusTree_Node *parent) {
  this->parent = parent;
  this->node_type = type;
  this->number_of_filled_units = 0;
}

int BPlusTree_Node::getNodeType() { return node_type; }

void BPlusTree_Node::setNumberOfFilledUnits(int num) {
  number_of_filled_units = num;
}

void BPlusTree_Node::incrementNumberOfFilledUnits() {
  number_of_filled_units++;
}

void BPlusTree_Node::decrementNumberOfFilledUnits() {
  number_of_filled_units--;
}

template <typename T>
BPlusTree_LeafNode<T>::BPlusTree_LeafNode() : BPlusTree_Node(LEAF_NODE) {}

template <typename T>
BPlusTree_InternalNode<T>::BPlusTree_InternalNode()
    : BPlusTree_Node(INTERNAL_NODE) {}

int BPlusTree_Node::getNumberOfFilledUnits() { return number_of_filled_units; }
template <typename T>
BPlusTree_InternalUnit<T>::BPlusTree_InternalUnit(
    T key, BPlusTree_InternalUnit *left, BPlusTree_InternalUnit *right) {
  this->key = key;
  this->left_child = left;
  this->right_child = right;
}

template <typename T>
BPlusTree_LeafUnit<T>::BPlusTree_LeafUnit(T key, void *reference,
                                          BPlusTree_LeafUnit *left,
                                          BPlusTree_LeafUnit *right) {
  this->key = key;
  this->reference = reference;
  this->left_child = left;
  this->right_child = right;
}

template <typename T>
BPlusTree_LeafNode<T> *BPlusTree<T>::findLeafNodeToInsert(T attrVal) {

  // get the root node
  BPlusTree_Node *nodePtr = root;

  while (nodePtr->getNodeType() == INTERNAL_NODE) {
    BPlusTree_InternalNode<T> *internalNode =
        (BPlusTree_InternalNode<T> *)nodePtr;

    int index = 0;
    for (; index < internalNode->num_units; index++) {
      if (attrVal < internalNode->units[index].key) { // TODO : DOUBLE CHECK
        break;
      }
    }
    // if no index is found

    if (index == internalNode->num_units) {
      nodePtr = internalNode->units[index].right_child;
    } else {
      nodePtr = internalNode->units[index].left_child;
    }
  }

  return (BPlusTree_LeafNode<T> *)nodePtr;
}

template <typename T>
int BPlusTree<T>::insertIntoLeaf(
    T attrVal, void *reference,
    BPlusTree_LeafNode<T> *leafNode) { // TODO : MAKE IT MORE MEMORY EFFICIENT

  int numEntries = leafNode->getNumberOfFilledUnits(), response;
  BPlusTree_LeafUnit<T> *indices[numEntries + 1];

  /*
  Iterate through all the entries in the block and copy them to the array
  indices. Also insert `indexEntry` at appropriate position in the indices array
  maintaining the ascending order.
  */
  int indicesArrayIndex = 0, recordEntry = 0;
  bool inserted = false;

  auto newEntry = new BPlusTree_LeafUnit<T>(attrVal, reference);

  for (; recordEntry < numEntries; recordEntry++) {

    indices[indicesArrayIndex] = &(leafNode->units[recordEntry]);

    // TODO : DOUBLE CHECK and IMPLEMENT COMPARE ATTRS
    // TODO : fix leaf node insertion. no left and right child for leaf
    if (!inserted && indices[indicesArrayIndex]->key >= attrVal) {
      indices[indicesArrayIndex + 1] = indices[indicesArrayIndex];
      indices[indicesArrayIndex] = newEntry;
      indicesArrayIndex++;
      numEntries++;
      inserted = true;
    }
    indicesArrayIndex++;
  }

  // if still not inserted, insert at the end
  // TODO : fix inserting into leaf node since leaf nodes do not have a left and
  // right child.(checl code below)
  if (!inserted) {
    indices[indicesArrayIndex] = newEntry;
    numEntries++;
  }

  // if leaf is full
  if (numEntries == MAX_KEYS_LEAF) {
    // split the leaf
    BPlusTree_LeafNode<T> *newRightChild = splitLeaf(indices, leafNode);

    // if the split failed
    if (newRightChild == NULL) {
      return MEMORY_FULL;
    }

    // if the current leaf was not root
    if (leafNode->parent) {
      // insert the middle value from `indices` into the parent block using the
      // insertIntoInternal() function. (i.e the last value of the left block)

      // the middle value will be at index 31 (given by constant
      // MIDDLE_INDEX_LEAF)

      // create a struct InternalEntry with attrVal =
      // indices[MIDDLE_INDEX_LEAF].attrVal, lChild = currentBlock, rChild =
      // newRightBlk and pass it as argument to

      response = insertIntoInternal(indices[MIDDLE_INDEX_LEAF]->key,
                                    leafNode->parent, leafNode, newRightChild);

      // if the insert fails
      if (response != SUCCESS) {
        return response;
      }

    } else { // if the current leaf was root
      // create a new root block with left and right children as current block
      // and newRightBlk using the createNewRoot() function
      createNewRoot(leafNode, newRightChild);

      // if the root creation fails
      if (root == NULL) {
        return MEMORY_FULL;
      }
    }
  } else { // if leaf is not full
    // copy the values from the indices array to the leaf block
    for (int i = 0; i < numEntries + 1; i++) {
      leafNode->units[i] = *indices[i];
    }
    leafNode->num_units++;
  }
  return SUCCESS;
}

template <typename T> int BPlusTree<T>::destroy() { return SUCCESS; }

template <typename T> int BPlusTree<T>::insert(T attrVal, void *reference) {

  // find the leaf block to which insertion is to be done using the
  // findLeafToInsert() function
  BPlusTree_LeafNode<T> *leafNode = findLeafNodeToInsert(attrVal);

  // insert the value into the leaf block using the insertIntoLeaf() function
  int response = insertIntoLeaf(attrVal, reference, leafNode);

  // if the insertion fails
  switch (response) {
  case MEMORY_FULL: {
    // destroy the tree
    destroy();
  }
    // TODO : Add more cases
  }
  return response;
}

template <typename T>
BPlusTree_LeafNode<T> *BPlusTree<T>::splitLeaf(BPlusTree_LeafNode<T> *leafNode,
                                               BPlusTree_LeafUnit<T> *indices) {

  // create a new leaf block
  BPlusTree_LeafNode<T> *newRightBlock = new BPlusTree_LeafNode<T>();

  newRightBlock->units[0].left_child = leafNode;

  // left number of entries
  const int leftNumberOfEntries = (MAX_KEYS_LEAF + 1) / 2;

  for (int i = 0; i < leftNumberOfEntries; ++i) {
    leafNode->units[i] = *indices[i];
    newRightBlock->units[i] = *indices[MIDDLE_INDEX_LEAF + i + 1];
  }

  // set new right child fields
  newRightBlock->setNumberOfFilledUnits((MAX_KEYS_LEAF + 1) / 2);
  newRightBlock->parent = leafNode->parent;

  // modify left child fields
  leafNode->setNumberOfFilledUnits(leftNumberOfEntries);

  return newRightBlock;
}

template <typename T>
int BPlusTree<T>::insertIntoInternal(T attrVal,
                                     BPlusTree_InternalNode<T> *internalNode,
                                     BPlusTree_Node *lChild,
                                     BPlusTree_Node *rChild) {
  int numEntries = internalNode->getNumberOfFilledUnits(), response;
  BPlusTree_InternalUnit<T> *indices[numEntries + 1];

  /*
  Iterate through all the entries in the block and copy them to the array
  indices. Also insert `indexEntry` at appropriate position in the indices array
  maintaining the ascending order.
  */
  int indicesArrayIndex = 0, recordEntry = 0;
  bool inserted = false;

  auto newEntry = new BPlusTree_InternalUnit<T>(attrVal, lChild, rChild);

  for (; recordEntry < numEntries; recordEntry++) {

    indices[indicesArrayIndex] = &(internalNode->units[recordEntry]);

    // TODO : DOUBLE CHECK and IMPLEMENT COMPARE ATTRS
    // add left and right children
    if (!inserted && indices[indicesArrayIndex]->key >= attrVal) {
      // newEntry->left_child = indices[indicesArrayIndex]->left_child;
      // newEntry->right_child = indices[indicesArrayIndex];
      // indices[indicesArrayIndex]->left_child = newEntry;
      // indices[indicesArrayIndex + 1] = indices[indicesArrayIndex];

      // indices[indicesArrayIndex] = newEntry;
      // indicesArrayIndex++;
      indices[indicesArrayIndex + 1] = indices[indicesArrayIndex];
      indices[indicesArrayIndex + 1]->left_child = newEntry->right_child;
      indices[indicesArrayIndex] = newEntry;
      indicesArrayIndex++;
      numEntries++;
      inserted = true;
    }
    indicesArrayIndex++;
  }

  // if still not inserted, insert at the end
  if (!inserted) {
    indices[indicesArrayIndex] = newEntry;
    numEntries++;
    // newEntry->right_child = indices[indicesArrayIndex - 1]->right_child;
    // newEntry->left_child = indices[indicesArrayIndex - 1];
    // newEntry->right_child = indices[indicesArrayIndex + 1];
  }

  // if leaf is full
  if (numEntries == MAX_KEYS_INTERNAL) {
    // split the leaf
    BPlusTree_LeafNode<T> *newRightChild = splitInternal(indices, internalNode);

    // if the split failed
    if (newRightChild == NULL) {
      return MEMORY_FULL;
    }

    // if the current leaf was not root
    if (internalNode->parent) {
      // insert the middle value from `indices` into the parent block using the
      // insertIntoInternal() function. (i.e the last value of the left block)

      // the middle value will be at index 31 (given by constant
      // MIDDLE_INDEX_INTERNAL)

      // create a struct InternalEntry with attrVal =
      // indices[MIDDLE_INDEX_INTERNAL].attrVal, lChild = currentBlock, rChild =
      // newRightBlk and pass it as argument to

      response =
          insertIntoInternal(indices[MIDDLE_INDEX_INTERNAL]->key,
                             internalNode->parent, internalNode, newRightChild);

      // if the insert fails
      if (response != SUCCESS) {
        return response;
      }

    } else { // if the current leaf was root
      // create a new root block with left and right children as current block
      // and newRightBlk using the createNewRoot() function
      createNewRoot(internalNode, newRightChild);

      // if the root creation fails
      if (this->root == NULL) {
        return MEMORY_FULL;
      }
    }
  } else { // if leaf is not full
    // copy the values from the indices array to the leaf block
    for (int i = 0; i < numEntries + 1; i++) {
      internalNode->units[i] = *indices[i];
    }
    internalNode->num_units++;
  }
  return SUCCESS;
}

template <typename T>
BPlusTree_InternalNode<T> *
BPlusTree<T>::splitInternal(BPlusTree_InternalNode<T> *internalNode,
                            BPlusTree_InternalUnit<T> *indices) {

  // create a new internal block
  BPlusTree_InternalNode<T> *newRightBlock = new BPlusTree_InternalNode<T>();

  newRightBlock->units[0].left_child = internalNode;

  // left number of entries
  const int leftNumberOfEntries = (MAX_KEYS_INTERNAL + 1) / 2;

  for (int i = 0; i < leftNumberOfEntries; ++i) {
    internalNode->units[i] = *indices[i];
    newRightBlock->units[i] = *indices[MIDDLE_INDEX_INTERNAL + i + 1];
  }

  // set new right child fields
  newRightBlock->setNumberOfFilledUnits((MAX_KEYS_INTERNAL + 1) / 2);
  newRightBlock->parent = internalNode->parent;

  // modify left child fields
  internalNode->setNumberOfFilledUnits(leftNumberOfEntries);

  return newRightBlock;
}

template <typename T>
void BPlusTree<T>::createNewRoot(BPlusTree_Node *lChild,
                                 BPlusTree_Node *rChild) {
  // create a new internal node
  BPlusTree_InternalNode<T> internalNode = new BPlusTree_InternalNode<T>();
  // set parent to NULL
  internalNode->parent = nullptr;
  internalNode->units[0].left_child = lChild;
  internalNode->units[0].right_child = rChild;
  internalNode.setNumberOfFilledUnits(1);

  lChild->setParent(internalNode);
  rChild->setParent(internalNode);

  this->root = internalNode;
}

template <typename T> int BPlusTree<T>::buildTree() {

  // TODO : Checks

  // Creating a new B+ Tree

  // create a new leaf node
  BPlusTree_LeafNode<T> *rootLeaf = new BPlusTree_LeafNode<T>(LEAF_NODE);

  // get morsel head
  Morsel *morselHead = this->relcat_entry.threadMap[this->coreNumber];

  // iterate through all the morsels
  // iterate through all the records in the morsel
  // get the number of entries
  Morsel *morselPtr = morselHead;
  int numEntriesFilled = morselPtr->getFilledNumberOfEntries();
  auto attributeList = this->relcat_entry.getAttributes();
  // get the morsel start
  // void *morselStart = m->getStartPtr();

  Tuple tup;
  // iterate through all morsels
  while (morselPtr) {
    // for each morsel, iterate through all entries
    for (int entryNo = 0; entryNo < numEntriesFilled; entryNo++) {
      void *entry = morselPtr->getNthMorselEntry(entryNo);
      void *value = tup.getTupleValue(this->attribute, entry);

      // insert into the tree
      int response = this->insert(value, entry);
      if (response != SUCCESS) {
        return response;
      }
    }
    // move to the next morsel
    morselPtr = morselPtr->getNext();
    if (morselPtr != nullptr) {
      numEntriesFilled = morselPtr->getFilledNumberOfEntries();
    }
  }
}
