//
//  BPlusTree.hpp
//  BPlusTree
//
//  Created by Chang Yin on 4/27/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#ifndef BPlusTree_hpp
#define BPlusTree_hpp

#include <stdio.h>
#include <iostream>
#include <deque>

using namespace std;

#define ORDER 5
#define MAX_POINTER ORDER
#define MAX_KEY (ORDER - 1)
#define MID_V MAX_KEY / 2
#define LEFT 1
#define RIGHT 2


enum NODE_TYPE {
    ROOT_NODE = 1,
    INNER_NODE = 2,
    LEAF_NODE = 3,
};

// ================================================================================================
// TreeNode class
// the superclass of InnerNode class and LeafNode class

class TreeNode {
protected:
    NODE_TYPE type; // what type of node
    int count; // number of valid key, for inner node is number of key
    TreeNode* parent; // pointer to parent node
public:
    TreeNode();
    ~TreeNode();
    // get and set Node type
    NODE_TYPE getType() { return type;}
    void setType(NODE_TYPE node_type) {type = node_type;}
    
    //get and set valid Node number
    int getCount() {return count;}
    void setCount(int num){count = num;}
    
    // abstract method for setting and getting key
    virtual int getKey(int i) {return 0;}
    virtual void setKey(int i, int key_value) {}
    
    // abstract method for setting and getting pointer
    virtual TreeNode* getPointer(int i){return 0;}
    virtual void setPointer(int i, TreeNode* pointer){}
    
    // getting and setting parent
    TreeNode* getParent(){return parent;}
    void setParent(TreeNode* parent_node){parent = parent_node;}
    // find a closest TreeNode to this TreeNode
    TreeNode* getBrother(int& flag);
    // print TreeNode
    void printNode();
    void deleteNode();
};

// ================================================================================================
// InnerNode class
// Inherit TreeNode class

class InnerNode : public TreeNode {
protected:
    int keys[MAX_KEY]; // An array of keys, number of keys is MAX_KEY
    TreeNode* pointers[MAX_POINTER]; // An array of pointers, number of pointers is MAX_POINTER
public:
    InnerNode();
    ~InnerNode();
    int getKey(int i) {
        if ((i < 0) || (i >= MAX_KEY)) {
            return 0;
        } else {
            return keys[i];
        }
    }
    void setKey(int i, int key_value) {
        if ((i >= 0) && (i < MAX_KEY)) {
            keys[i] = key_value;
        }
    }
    TreeNode* getPointer(int i) {
        if ((i < 0) || (i >= MAX_POINTER)) {
            return 0;
        } else {
            return pointers[i];
        }
    }
    void setPointer(int i, TreeNode* pointer) {
        if ((i >= 0) && (i < MAX_POINTER)) {
            pointers[i] = pointer;
        }
    }
    bool insert(int value, TreeNode* pNode);
    bool remove(int value);
    int split(InnerNode* pNode, int key);
    bool merge(TreeNode* pNode);
    // Move one key from pNode to this InnerNode
    bool moveOneKey(TreeNode* pNode);
};

// ================================================================================================
// LeafNode class
// Inherit TreeNode class

class LeafNode : public TreeNode {
protected:
    // here we just assume the data is key
    int data[MAX_KEY]; // An array of data, number of data is MAX_KEY
public:
    // double linked list
    LeafNode* prev;
    LeafNode* next;
public:
    LeafNode();
    ~LeafNode();
    int getKey(int i) {
        if ((i < 0) || (i >= MAX_KEY)) {
            return 0;
        } else {
            return data[i];
        }
    }
    void setKey(int i, int key_value) {
        if ((i >= 0) && (i < MAX_KEY)) {
            data[i] = key_value;
        }
    }
    TreeNode* getPointer(int i) {
        return 0;
    }
    
    bool insert(int value);
    bool remove(int value);
    int split(LeafNode* newLeaf);
    bool merge(TreeNode* pNode);
};

// ================================================================================================
// BPlusTree class

class BPlusTree {
public:
    TreeNode* root; // root of B Plus Tree
    // head and tail of leave node
    LeafNode* head;
    LeafNode* tail;
public:
    BPlusTree();
    ~BPlusTree();
    bool search(int key); // search the key
    bool insert(int key); // insert the key
    bool remove(int key); // remove the key
    void printTree();
    void clearTree();
private:
    LeafNode* searchLeaf(int key);
    bool insertInnerNode(InnerNode* pNode, int key, TreeNode* rightSon);
    bool removeInnerNode(InnerNode* pNode, int key);
};

#endif /* BPlusTree_hpp */
