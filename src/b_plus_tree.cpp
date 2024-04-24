// #include "b_plus_tree.h"

// B_Plus_Tree_Node::B_Plus_Tree_Node(bool leaf)
// {
//     is_leaf = leaf;
//     parent = nullptr;
// }

// B_Plus_Tree_Node *BPlusTree::create_leaf_node()
// {
//     B_Plus_Tree_Node *node = new B_Plus_Tree_Node(true);
//     return node;
// }

// B_Plus_Tree_Node *BPlusTree::create_internal_node()
// {
//     B_Plus_Tree_Node *node = new B_Plus_Tree_Node(false);
//     return node;
// }

// void BPlusTree::split_child(B_Plus_Tree_Node *parent, int index)
// {
//     B_Plus_Tree_Node *child = parent->children[index];
//     B_Plus_Tree_Node *new_child = create_internal_node();
//     new_child->parent = parent;

//     // Move the right half of the keys and children to the new child
//     new_child->keys.assign(child->keys.begin() + (MAX_B_PLUS_TREE_KEYS / 2), child->keys.end());
//     child->keys.resize(MAX_B_PLUS_TREE_KEYS / 2);

//     if (!child->is_leaf)
//     {
//         new_child->children.assign(child->children.begin() + (MAX_B_PLUS_TREE_KEYS / 2), child->children.end());
//         child->children.resize(MAX_B_PLUS_TREE_KEYS / 2 + 1);
//     }

//     // Insert the middle key to the parent
//     int mid_key = new_child->keys[0];
//     int parent_insert_pos = 0;
//     for (; parent_insert_pos < parent->keys.size(); ++parent_insert_pos)
//     {
//         if (parent->keys[parent_insert_pos] > mid_key)
//         {
//             break;
//         }
//     }
//     parent->keys.insert(parent->keys.begin() + parent_insert_pos, mid_key);
//     parent->children.insert(parent->children.begin() + parent_insert_pos + 1, new_child);
// }

// BPlusTree::BPlusTree()
// {
//     root = create_leaf_node();
// }

// void BPlusTree::insert(int key)
// {
//     B_Plus_Tree_Node *node = root;
//     if (node->keys.size() == MAX_B_PLUS_TREE_KEYS)
//     {
//         B_Plus_Tree_Node *new_root = create_internal_node();
//         new_root->children.push_back(root);
//         root->parent = new_root;
//         root = new_root;
//         split_child(new_root, 0);
//         node = new_root;
//     }

//     while (!node->is_leaf)
//     {
//         int index = 0;
//         while (index < node->keys.size() && node->keys[index] < key)
//         {
//             ++index;
//         }
//         if (index > 0 && node->keys[index - 1] == key)
//         {
//             return; // Key already exists
//         }
//         node = node->children[index];
//         if (node->keys.size() == MAX_B_PLUS_TREE_KEYS)
//         {
//             split_child(node->parent, index);
//             node = node->parent->children[index + 1];
//         }
//     }

//     // Insert the key into the leaf node
//     int insert_pos = 0;
//     while (insert_pos < node->keys.size() && node->keys[insert_pos] < key)
//     {
//         ++insert_pos;
//     }
//     node->keys.insert(node->keys.begin() + insert_pos, key);
// }

// void BPlusTree::print_tree_fn(B_Plus_Tree_Node *node, int level)
// {
//     if (node != nullptr)
//     {
//         for (int i = 0; i < node->keys.size(); ++i)
//         {
//             std::cout << node->keys[i] << " ";
//         }
//         std::cout << std::endl;

//         if (!node->is_leaf)
//         {
//             for (int i = 0; i < node->children.size(); ++i)
//             {
//                 print_tree_fn(node->children[i], level + 1);
//             }
//         }
//     }
// }

// void BPlusTree::print_tree()
// {
//     print_tree_fn(root);
// }
