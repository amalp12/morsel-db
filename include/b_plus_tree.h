#pragma once
#include "attribute.h"
#include "constants.h"
#include "relcat.h"

class BPlusTree_Node {
private:
  int node_type; // 0 for leaf, 1 for internal
  int number_of_filled_units;

public:
  // define constructor
  BPlusTree_Node *parent;
  BPlusTree_Node(int type);
  BPlusTree_Node(int type, BPlusTree_Node *parent);
  // define node type getter
  int getNodeType();
  int getNumberOfFilledUnits();
  void setNumberOfFilledUnits(int num);
  void incrementNumberOfFilledUnits();
  void decrementNumberOfFilledUnits();
  BPlusTree_Node *getParent();
  void setParent(BPlusTree_Node *parent);
};

template <typename T> class BPlusTree_NodeUnit {
public:
  T key;
  BPlusTree_Node *left_child;
  BPlusTree_Node *right_child;
};

// Leaf Unit of type TK for key and  TV for value
template <typename T> class BPlusTree_LeafUnit : public BPlusTree_NodeUnit<T> {
public:
  void *reference;
  // constructor, define key, reference, left and right child with default
  // values
  BPlusTree_LeafUnit(T key, void *reference, BPlusTree_Node *left,
                     BPlusTree_Node *right);
  BPlusTree_LeafUnit();
};

// Internal Unit
template <typename T>
class BPlusTree_InternalUnit : public BPlusTree_NodeUnit<T> {
public:
  // constructor, define key, left and right child with default values
  BPlusTree_InternalUnit(T key, BPlusTree_Node *left = NULL,
                         BPlusTree_Node *right = NULL);
  BPlusTree_InternalUnit();
};

// Leaf Node
template <typename T> class BPlusTree_LeafNode : public BPlusTree_Node {
public:
  BPlusTree_LeafNode<T> *next;
  BPlusTree_LeafNode<T> *prev;
  BPlusTree_LeafNode<T>();
  BPlusTree_LeafUnit<T> units[MAX_KEYS_LEAF];
};

// Internal Node
template <typename T> class BPlusTree_InternalNode : public BPlusTree_Node {
public:
  BPlusTree_InternalNode<T>();
  BPlusTree_InternalUnit<T> units[MAX_KEYS_INTERNAL];
};

template <typename T> class BPlusTree {
private:
  BPlusTree_Node *root;
  Attribute *attribute;
  RelationCatalogEntry *relcat_entry;
  int coreNumber;

  // find leaf to insert
  BPlusTree_LeafNode<T> *findLeafNodeToInsert(T attrVal);

  // insert into leaf
  int insertIntoLeaf(T attrVal, void *reference,
                     BPlusTree_LeafNode<T> *leafNode);

  // split leaf
  BPlusTree_LeafNode<T> *splitLeaf(BPlusTree_LeafNode<T> *leafNode,
                                   BPlusTree_LeafUnit<T> **indices);

  // insert into internal
  int insertIntoInternal(T attrVal, BPlusTree_InternalNode<T> *internalNode,
                         BPlusTree_Node *lChild, BPlusTree_Node *rChild);

  // split internal
  BPlusTree_InternalNode<T> *
  splitInternal(BPlusTree_InternalNode<T> *internalNode,
                BPlusTree_InternalUnit<T> **indices);

  // create new root
  void createNewRoot(BPlusTree_Node *lChild, BPlusTree_Node *rChild);

public:
  // create/build tree
  int buildTree();
  // constructor
  BPlusTree(Attribute *attr, RelationCatalogEntry *entry, int core);

  // insert
  int insert(T attrVal, void *reference);

  // search
  void *search(T attrVal, int op);

  int destroy();

  // destructor
  ~BPlusTree();
};

// template class BPlusTree<double>;
template class BPlusTree<int>;
template class BPlusTree<std::string>;

// SELECT Table_A.Column_1_Table_A ,
// Table_B.Column_1_Table_B,Table_A.Column_3_Table_A , Table_B.Column_3_Table_B
// FROM Table_A INNER JOIN Table_B ON Table_A.Column_3_Table_A =
// Table_B.Column_3_Table_B;