#include "attribute.h"
#include "constants.h"
#include "relcat.h"
#include <iostream>
#include <vector>

class BPlusTree_Node {
private:
  bool node_type; // 0 for leaf, 1 for internal

public:
  // define constructor
  BPlusTree_Node(bool type) { node_type = type; }
  // define node type getter
  bool get_node_type() { return node_type; }
};

// Leaf Unit of type TK for key and  TV for value
template <typename T> class BPlusTree_LeafUnit {
public:
  T key;
  void *reference;
  BPlusTree_LeafUnit *next;
};

// Internal Unit
template <typename T> class BPlusTree_InternalUnit {
public:
  T key;
  BPlusTree_Node *right_child;
};

// Leaf Node
template <typename T> class BPlusTree_LeafNode : public BPlusTree_Node {
public:
  BPlusTree_LeafUnit<T> units[MAX_LEAF_NODE_SIZE];
};

// Internal Node
template <typename T> class BPlusTree_InternalNode : public BPlusTree_Node {
public:
  BPlusTree_InternalUnit<T> units[MAX_INTERNAL_NODE_SIZE];
};

template <typename T> class BPlusTree {
private:
  BPlusTree_Node *root;
  Attribute attribute;
  RelationCatalogEntry relcat_entry;

  // find leaf to insert
  BPlusTree_LeafNode<T> *findLeafNodeToInsert(T attrVal);

  // insert into leaf
  int insertIntoLeaf(T attrVal, BPlusTree_LeafNode<T> *leafNode);

  // split leaf
  BPlusTree_LeafNode<T> *splitLeaf(BPlusTree_LeafNode<T> *leafNode);

  // insert into internal
  int insertIntoInternal(T attrVal, BPlusTree_InternalNode<T> *internalNode);

  // split internal
  BPlusTree_InternalNode<T> *
  splitInternal(BPlusTree_InternalNode<T> *internalNode);

  // create new root
  BPlusTree_Node *createNewRoot(Attribute *attr, BPlusTree_Node *lChild,
                                BPlusTree_Node *rChild);

  // create/build tree
  int buildTree();

public:
  // constructor
  BPlusTree(Attribute attr, RelationCatalogEntry entry) {
    root = new BPlusTree_LeafNode<T>(LEAF_NODE);
    // build
    attribute = attr;
    relcat_entry = entry;
  }

  // insert

  // search

  // destructor
  ~BPlusTree();
};

// private:
//   static int findLeafToInsert(int rootBlock, Attribute attrVal, int
//   attrType); static int insertIntoLeaf(int relId, char attrName[ATTR_SIZE],
//   int blockNum, Index entry); static int splitLeaf(int leafBlockNum, Index
//   indices[]); static int insertIntoInternal(int relId, char
//   attrName[ATTR_SIZE], int intBlockNum, InternalEntry entry); static int
//   splitInternal(int intBlockNum, InternalEntry internalEntries[]); static int
//   createNewRoot(int relId, char attrName[ATTR_SIZE], Attribute attrVal, int
//   lChild, int rChild);

//  public:
//   static int bPlusCreate(int relId, char attrName[ATTR_SIZE]);
//   static int bPlusInsert(int relId, char attrName[ATTR_SIZE], union Attribute
//   attrVal, RecId recordId); static RecId bPlusSearch(int relId, char
//   attrName[ATTR_SIZE], union Attribute attrVal, int op); static int
//   bPlusDestroy(int rootBlockNum);