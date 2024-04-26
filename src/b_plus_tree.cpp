#include "b_plus_tree.h"
#include "constants.h"
#include "lib.h"
#include "tuple.h"

// constructor
template <typename T>
BPlusTree<T>::BPlusTree(Attribute *attr, RelationCatalogEntry *entry,
                        int core) {
  root = new BPlusTree_LeafNode<T>();
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
  this->number_of_filled_units = num;
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

template <typename T> BPlusTree_InternalUnit<T>::BPlusTree_InternalUnit() {}

template <typename T>
BPlusTree_InternalUnit<T>::BPlusTree_InternalUnit(T key, BPlusTree_Node *left,
                                                  BPlusTree_Node *right)
    : BPlusTree_NodeUnit<T>() {
  this->key = key;
  this->left_child = left;
  this->right_child = right;
}
template <typename T> BPlusTree_LeafUnit<T>::BPlusTree_LeafUnit() {}
template <typename T>
BPlusTree_LeafUnit<T>::BPlusTree_LeafUnit(T key, void *reference,
                                          BPlusTree_Node *left,
                                          BPlusTree_Node *right)
    : BPlusTree_NodeUnit<T>() {
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
    for (; index < internalNode->getNumberOfFilledUnits(); index++) {
      if (attrVal < internalNode->units[index].key) { // TODO : DOUBLE CHECK
        break;
      }
    }
    // if no index is found

    if (index == internalNode->getNumberOfFilledUnits()) {
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

  auto newEntry =
      new BPlusTree_LeafUnit<T>(attrVal, reference, nullptr, nullptr);

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
    BPlusTree_LeafNode<T> *newRightChild = splitLeaf(leafNode, indices);

    // if the split failed
    if (newRightChild == NULL) {
      return MEMORY_FULL;
    }

    leafNode->next = newRightChild;
    newRightChild->prev = leafNode;

    // if the current leaf was not root
    if (leafNode->parent) {
      // insert the middle value from `indices` into the parent block using the
      // insertIntoInternal() function. (i.e the last value of the left block)

      // the middle value will be at index 31 (given by constant
      // MIDDLE_INDEX_LEAF)

      // create a struct InternalEntry with attrVal =
      // indices[MIDDLE_INDEX_LEAF].attrVal, lChild = currentBlock, rChild =
      // newRightBlk and pass it as argument to

      response =
          insertIntoInternal(indices[MIDDLE_INDEX_LEAF]->key,
                             (BPlusTree_InternalNode<T> *)leafNode->parent,
                             leafNode, newRightChild);

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
    for (int i = 0; i < numEntries; i++) {
      leafNode->units[i] = *indices[i];
    }
    leafNode->incrementNumberOfFilledUnits();
  }
  return SUCCESS;
}

template <typename T> int BPlusTree<T>::destroy() { return SUCCESS; }

template <typename T> void *BPlusTree<T>::search(T attrVal, int op) {

  // declare Node pointer
  BPlusTree_Node *nodePtr = this->root;
  BPlusTree_InternalUnit<T> *internalUnit;

  // while the node is not a leaf node
  while (nodePtr->getNodeType() == INTERNAL_NODE) {
    // typecast the node to internal node
    BPlusTree_InternalNode<T> *internalNode =
        (BPlusTree_InternalNode<T> *)nodePtr;

    // op is one of NE, LT, LE
    if (op == NOT_EQUAL || op == LESS_THAN || op == LESS_THAN_OR_EQUAL) {
      // - NE: need to search the entire linked list of leaf indices of the B+
      // Tree, starting from the leftmost leaf index. Thus, always move to the
      // left.

      // - LT and LE: the attribute values are arranged in ascending order in
      // the leaf indices of the B+ Tree. Values that satisfy these conditions,
      // if any exist, will always be found in the left-most leaf index. Thus,
      // always move to the left.

      nodePtr = internalNode->units[0].left_child;
    } else {
      // - EQ, GT and GE: move to the left child of the first entry that is
      //       greater than (or equal to) attrVal
      //       (we are trying to find the first entry that satisfies the
      //       condition. since the values are in ascending order we move to the
      //       left child which might contain more entries that satisfy the
      //       condition)

      // traverse through all entries of internalBlk and find an entry that
      //        satisfies the condition.
      //        if op == EQ or GE, then intEntry.attrVal >= attrVal
      //        if op == GT, then intEntry.attrVal > attrVal
      //        Hint: the helper function compareAttrs() can be used for
      //        comparing

      int index = 0, numberOfUnits = internalNode->getNumberOfFilledUnits();
      while (index < numberOfUnits) {

        // TODO : Implement Compare Attrs
        int cmpVal; //= compareAttrs(internalNode->units[index].key, attrVal);
        if (((op == EQUAL || op == GREATER_THAN_OR_EQUAL) && cmpVal >= 0) ||
            (op == GREATER_THAN && cmpVal > 0)) {
          break;
        }
        index++;
      }

      /* such an entry is found*/
      if (index < numberOfUnits) {
        // move to the left child of that entry
        // left child of the entry
        nodePtr = internalNode->units[index].left_child;

      } else {
        // move to the right child of the last entry of the block
        // i.e numEntries - 1 th entry of the block

        // right child of last entry
        nodePtr = internalNode->units[numberOfUnits - 1].right_child;
      }
    }
  }

  // NOTE: `nodePtr` now has the block number of a leaf index Node.

  /******  Identify the first leaf index entry from the current position
              that satisfies our condition (moving right)             ******/

  // typecast the node to leaf node
  BPlusTree_LeafNode<T> *leafNode = (BPlusTree_LeafNode<T> *)nodePtr;

  while (leafNode) {
    int numberOfUnits = leafNode->getNumberOfFilledUnits();
    for (int index = 0; index < numberOfUnits; index++) {

      // TODO : Implement compareAttrs
      int cmpVal = compareAttrs(leafNode->units[index].key, attrVal, op);

      // calculate cmpVal fro integer and string
      // switch (this->attribute->type) {

      // case INTEGER: {
      //   cmpVal = attrVal - leafNode->units[index].key;
      //   break;
      // }

      // case STRING: {
      //   // typcast `t *` to `std::string`
      //   std::string stringVal = (std::string)attrVal;

      //   cmpVal = stringVal.compare(leafNode->units[index].key);
      //   break;
      // }
      // }
      if ((op == EQUAL && cmpVal == 0) ||
          (op == LESS_THAN_OR_EQUAL && cmpVal <= 0) ||
          (op == LESS_THAN && cmpVal < 0) ||
          (op == GREATER_THAN && cmpVal > 0) ||
          (op == GREATER_THAN_OR_EQUAL && cmpVal >= 0) ||
          (op == NOT_EQUAL && cmpVal != 0)) {

        // return the reference
        return leafNode->units[index].reference;

      } else if ((op == EQUAL || op == LESS_THAN_OR_EQUAL || op == LESS_THAN) &&
                 cmpVal > 0) {
        /*future entries will not satisfy EQ, LE, LT since the values
            are arranged in ascending order in the leaves */
        return nullptr;
      }

      /*only for NE operation do we have to check the entire linked list;
          for all the other op it is guaranteed that the block being searched
          will have an entry, if it exists, satisying that op. */
      if (op != NOT_EQUAL) {
        break;
      }
      // block = next block in the linked list, i.e., the rblock in leafHead.
    }
    leafNode = leafNode->next;
  }

  return nullptr;
}

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
BPlusTree_LeafNode<T> *
BPlusTree<T>::splitLeaf(BPlusTree_LeafNode<T> *leafNode,
                        BPlusTree_LeafUnit<T> **indices) {

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
  leafNode->next = newRightBlock;
  newRightBlock->prev = leafNode;
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
  indices. Also insert `indexEntry` at appropriate position in the indices
  array maintaining the ascending order.
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
    BPlusTree_InternalNode<T> *newRightChild =
        splitInternal(internalNode, indices);

    // if the split failed
    if (newRightChild == NULL) {
      return MEMORY_FULL;
    }

    // if the current leaf was not root
    if (internalNode->parent) {
      // insert the middle value from `indices` into the parent block using
      // the insertIntoInternal() function. (i.e the last value of the left
      // block)

      // the middle value will be at index 31 (given by constant
      // MIDDLE_INDEX_INTERNAL)

      // create a struct InternalEntry with attrVal =
      // indices[MIDDLE_INDEX_INTERNAL].attrVal, lChild = currentBlock, rChild
      // = newRightBlk and pass it as argument to

      response =
          insertIntoInternal(indices[MIDDLE_INDEX_INTERNAL]->key,
                             (BPlusTree_InternalNode<T> *)internalNode->parent,
                             internalNode, newRightChild);

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
    for (int i = 0; i < numEntries; i++) {
      internalNode->units[i] = *indices[i];
    }
    internalNode->incrementNumberOfFilledUnits();
  }
  return SUCCESS;
}

template <typename T>
BPlusTree_InternalNode<T> *
BPlusTree<T>::splitInternal(BPlusTree_InternalNode<T> *internalNode,
                            BPlusTree_InternalUnit<T> **indices) {

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
  BPlusTree_InternalNode<T> *internalNode = new BPlusTree_InternalNode<T>();
  // set parent to NULL
  internalNode->parent = nullptr;
  internalNode->units[0].left_child = lChild;
  internalNode->units[0].right_child = rChild;
  internalNode->setNumberOfFilledUnits(1);

  lChild->setParent(internalNode);
  rChild->setParent(internalNode);

  this->root = internalNode;
}

template <typename T> int BPlusTree<T>::buildTree() {

  // TODO : Checks

  // Creating a new B+ Tree

  // create a new leaf node
  BPlusTree_LeafNode<T> *rootLeaf = new BPlusTree_LeafNode<T>();

  // get morsel head
  Morsel *morselHead = this->relcat_entry->threadMap[this->coreNumber];

  // iterate through all the morsels
  // iterate through all the records in the morsel
  // get the number of entries
  Morsel *morselPtr = morselHead;
  int numEntriesFilled = morselPtr->getFilledNumberOfEntries();
  auto attributeList = this->relcat_entry->getAttributes();
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
      int response = this->insert(*(T *)value, entry);
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
  return SUCCESS;
}
