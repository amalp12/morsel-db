#include <iostream>
#include <vector>
#include "constants.h"
#include "relcat.h"

class BPlusTree_Node
{
private:
    bool node_type; // 0 for leaf, 1 for internal

public:
    // define constructor
    BPlusTree_Node(bool type)
    {
        node_type = type;
    }
    // define node type getter
    bool get_node_type()
    {
        return node_type;
    }
};

// Leaf Unit of type TK for key and  TV for value
template <typename TK, typename TV>
class BPlusTree_LeafUnit
{
    TK key;
    TV value;
    BPlusTree_LeafUnit *next;
};

// Internal Unit
template <typename T>
class BPlusTree_InternalUnit
{
    T key;
    BPlusTree_Node<T> *right_child;
};

// Leaf Node
template <typename TK, typename TV>
class BPlusTree_LeafNode : public BPlusTree_Node<T>
{
    BPlusTree_LeafUnit<TK, TV> units[MAX_LEAF_NODE_SIZE];
};

// Internal Node
template <typename T>
class BPlusTree_InternalNode : public BPlusTree_Node<T>
{
    BPlusTree_InternalUnit<T> units[MAX_INTERNAL_NODE_SIZE];
    BPlusTree_Node<T> *next;
};

template <typename TK, typename TV>
class BPlusTree
{
private:
    BPlusTree_Node<TK> *root;
    Attribute attribute;
    RelationCatalogEntry relcat_entry;

    // find leaf to insert
    BPlusTree_LeafNode * findLeafNodeToInsert(TV attrVal);

    // insert into leaf
    int insertIntoLeaf(TV attrVal, BPlusTree_LeafNode *leafNode);

    // split leaf
    BPlusTree_LeafNode * splitLeaf(BPlusTree_LeafNode *leafNode);

    // insert into internal
    int insertIntoInternal(TV attrVal, BPlusTree_InternalNode *internalNode);

    // split internal
    BPlusTree_InternalNode * splitInternal(BPlusTree_InternalNode *internalNode);

    // create new root

    

    // create/build tree






public:
    // constructor
    BPlusTree(Attribute attr, RelationCatalogEntry entry)
    {
        root = new BPlusTree_LeafNode<TK, TV>(LEAF_NODE);
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
//   static int findLeafToInsert(int rootBlock, Attribute attrVal, int attrType);
//   static int insertIntoLeaf(int relId, char attrName[ATTR_SIZE], int blockNum, Index entry);
//   static int splitLeaf(int leafBlockNum, Index indices[]);
//   static int insertIntoInternal(int relId, char attrName[ATTR_SIZE], int intBlockNum, InternalEntry entry);
//   static int splitInternal(int intBlockNum, InternalEntry internalEntries[]);
//   static int createNewRoot(int relId, char attrName[ATTR_SIZE], Attribute attrVal, int lChild, int rChild);

//  public:
//   static int bPlusCreate(int relId, char attrName[ATTR_SIZE]);
//   static int bPlusInsert(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, RecId recordId);
//   static RecId bPlusSearch(int relId, char attrName[ATTR_SIZE], union Attribute attrVal, int op);
//   static int bPlusDestroy(int rootBlockNum);