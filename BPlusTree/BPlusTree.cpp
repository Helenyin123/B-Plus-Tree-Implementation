//
//  BPlusTree.cpp
//  BPlusTree
//
//  Created by Chang Yin on 4/27/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#include "BPlusTree.hpp"

// ================================================================================================
// TreeNode class
// the superclass of InnerNode class and LeafNode class

TreeNode::TreeNode() {
    type = LEAF_NODE;
    count = 0;
    parent = 0;
}
TreeNode::~TreeNode() {
    deleteNode();
}
// print a TreeNode
void TreeNode::printNode() {
    for (int i = 0; i < this->getCount(); i++) {
        printf("%d ", this->getKey(i));
    }
    printf(" | ");
}
void TreeNode::deleteNode() {
    for (int i = 0; i <= count; i++) {
        TreeNode* pNode = this->getPointer(i);
        if (pNode != 0) {
            pNode->deleteNode();
        }
        delete pNode;
    }
}
// get the closest TreeNode to this TreeNode
TreeNode* TreeNode::getBrother(int &flag) {
    TreeNode* parent = this->getParent();
    // If this TreeNode don't have parent, then we can't find brother node
    if (parent == 0) {
        return 0;
    }
    TreeNode* pBrother = 0;
    for (int i = 0; i <= parent->getCount(); i++) {
        // find the pointer index of this TreeNode
        if (parent->getPointer(i) == this) {
            // If this TreeNode is the last pointer, then look for its left brother node
            if (i == parent->getCount()) {
                pBrother = parent->getPointer(i - 1);
                flag = LEFT;
            // else look for this TreeNode's right brother node first
            } else {
                pBrother = parent->getPointer(i + 1);
                flag = RIGHT;
            }
        }
    }
    return pBrother;
}

// ================================================================================================
// InnerNode class
// Inherit TreeNode class

InnerNode::InnerNode() {
    type = INNER_NODE;
    for (int i = 0; i < MAX_KEY; i++) {
        keys[i] = 0;
    }
    for(int i = 0; i < MAX_POINTER; i++) {
        pointers[i] = 0;
    }
}
InnerNode::~InnerNode() {
    for (int i = 0; i < MAX_POINTER; i++) {
        pointers[i] = 0;
    }
}

// Asssume there is no duplicate key
bool InnerNode::insert(int value, TreeNode* pNode) {
    if (getCount() >= MAX_KEY) {
        return false;
    }
    int i = 0;
    // find insert position
    while ((keys[i] < value) && (i < count)) {i++;}
    // copy all keys after index i to count
    for (int j = count; j > i; j--) {
        keys[j] = keys[j - 1];
    }
    // copy all pointers after index i + 1 to count + 1
    for (int j = count + 1; j > i + 1; j--) {
        pointers[j] = pointers[j - 1];
    }
    // insert the key to key[i] and pointer to pointers[i + 1]
    keys[i] = value;
    pointers[i + 1] = pNode;
    // set parent pointer and count + 1
    pNode->setParent(this);
    count++;
    
    return true;
}

// return true if remove it successfully
bool InnerNode::remove(int value) {
    int i = 0;
    // find the position of key == value
    while ((keys[i] < value) && (i < count)) {i++;}
    // if no key == value, return false
    if (keys[i] != value) {
        return false;
    }
    // move all key after position i forward
    int j = 0;
    for (j = i; j < count - 1; j++) {
        keys[j] = keys[j + 1];
    }
    keys[j] = 0;
    int k = 0;
    // move all pointers after i forward
    for(k = i + 1; k < count; k++) {
        pointers[k] = pointers[k + 1];
    }
    pointers[k] = 0;
    count--;
    return true;
}

/*
 1. if key < keys[MID_V - 1], get [MID_V - 1]th key out, split its left and right
 2. if key > keys[MID_V], get [MID_V]th key out, split its left and right
 3. if keys[MID_V - 1] < key < keys[MID_V], get key out, split its left and right
 */
int InnerNode::split(InnerNode* pNode, int key) {
    // first handle case 3
    if ((key > this->getKey(MID_V - 1)) && (key < this->getKey(MID_V))) {
        int j = 0;
        // copy all [MID_V, MAX_KEY] keys to new InnerNode
        for (int i = MID_V; i < MAX_KEY; i++) {
            pNode->setKey(j, keys[i]);
            this->setKey(i, 0);
            j++;
        }
        j = 0;
        // copy all pointers after MID_V + 1
        for (int i = MID_V + 1; i < MAX_POINTER; i++) {
            // pNode is the parent node of all in next layer
            this->getPointer(i)->setParent(pNode);
            pNode->setPointer(j, this->getPointer(i));
            this->setPointer(i, 0);
            j++;
        }
        // set count number
        this->setCount(MID_V);
        pNode->setCount(MID_V);
        // return key
        return key;
    }
    // handle case 1 and 2 together
    int position = 0;
    if (key < this->getKey(MID_V - 1)) {
        position = MID_V - 1;
    } else {
        position = MID_V;
    }
    int newParent = this->getKey(position);
    int j = 0;
    // copy all keys after position to new leafnode
    for (int i = position + 1; i < MAX_KEY; i++) {
        pNode->setKey(j, this->getKey(i));
        this->setKey(i, 0);
        j++;
    }
    j = 0;
    // copy all pointers after position
    for (int i = position + 1; i < MAX_POINTER; i++) {
        this->getPointer(i)->setParent(pNode);
        pNode->setPointer(j, this->getPointer(i));
        this->setPointer(i, 0);
        j++;
    }
    this->setKey(position, 0);
    this->setCount(position);
    pNode->setCount(MAX_KEY - position - 1);
    return newParent;
}
// Move one key from pNode to this node
bool InnerNode:: moveOneKey(TreeNode* pNode) {
    if (this->getCount() >= MAX_KEY) {
        return false;
    }
    // pNode is left brother to this node
    if (pNode->getKey(0) < this->getKey(0)) {
        for (int i = count; i > 0; i--) {
            keys[i] = keys[i - 1];
        }
        for (int j = count + 1; j > 0; j--) {
            pointers[j] = pointers[j - 1];
        }
        keys[0] = this->getPointer(0)->getKey(0);
        pointers[0] = pNode->getPointer(pNode->getCount());
        pointers[0]->setParent(this);
        
        // pNode's last Key and pointer is set to invalid
        pNode->setKey(pNode->getCount() - 1, 0);
        pNode->setPointer(pNode->getCount(), 0);
    // pNode is right brother to this node
    } else {
        // move brother node's first key and pointer to this node's last key and pointer
        keys[count - 1] = pNode->getPointer(0)->getKey(0);
        pointers[count] = pNode->getPointer(0);
        pointers[count]->setParent(this);
        // pNode's first key and pointer is removed
        for (int i = 0; i < pNode->getCount(); i++) {
            pNode->setKey(i, pNode->getKey(i + 1));
        }
        for (int j = 0; j < pNode->getCount(); j++) {
            pNode->setPointer(j, pNode->getPointer(j + 1));
        }
    }
    // set count of this node and pNode
    this->setCount(this->getCount() + 1);
    pNode->setCount(pNode->getCount() - 1);
    return true;
}
bool InnerNode::merge(TreeNode* pNode) {
    if (this->getCount() + pNode->getCount() > MAX_KEY) {
        return false;
    }
    // To merge a InnerNode, we need to add a newKey which is the first key of first pointer of pNode
    int newKey = pNode->getPointer(0)->getKey(0);
    keys[count] = newKey;
    pointers[count + 1] = pNode->getPointer(0);
    pointers[count + 1]->setParent(this); // change parent node of pointer
    count++;
    for (int i = 0; i < pNode->getCount(); i++) {
        keys[count] = pNode->getKey(i);
        pointers[count + 1] = pNode->getPointer(i + 1);
        pointers[count + 1]->setParent(this); // change parent node of pointer
        count++;
    }
    return true;
}

// ================================================================================================
// LeafNode class
// Inherit TreeNode class

LeafNode::LeafNode() {
    type = LEAF_NODE;
    for (int i = 0; i < MAX_KEY; i++) {
        data[i] = 0;
    }
    prev = 0;
    next = 0;
}
LeafNode::~LeafNode() {}
bool LeafNode::insert(int value) {
    if (count >= MAX_KEY) {
        return false;
    }
    int i = 0;
    // find insert position i
    while ((data[i] < value) && (i < count)) {i++;}
    // move all keys after i  to index + 1
    for (int j = count; j > i; j--) {
        data[j] = data[j - 1];
    }
    // LeafNode don't have pointer array
    data[i] = value;
    count++;
    return true;
}
bool LeafNode::remove(int value) {
    int i = 0;
    // find the position where data[i] = value
    while ((data[i] < value) && (i < count)) {i++;}
    if (data[i] != value) {
        return false;
    }
    int j = i;
    for (j = i; j < count - 1; j++) {
        data[j] = data[j + 1];
    }
    data[j] = 0;
    count--;
    return true;
}

// split LeafNode, copy second half of original keys to newLeaf Node
int LeafNode::split(LeafNode* newLeaf) {
    int j = 0;
    // copy second half key
    for (int i = MAX_KEY / 2; i < MAX_KEY; i++) {
        // LeafNode don't have pointer array, don't need to copy pointers
        newLeaf->setKey(j, this->getKey(i));
        this->setKey(i, 0);
        j++;
    }
    this->setCount(this->getCount() - j);
    newLeaf->setCount(newLeaf->getCount() + j);
    // return the first key of newLeaf
    return newLeaf->getKey(0);
}

bool LeafNode::merge(TreeNode* pNode) {
    if (this->getCount() + pNode->getCount() > MAX_KEY) {
        return false;
    }
    for (int i = 0; i < pNode->getCount(); i++) {
        this->insert(pNode->getKey(i));
    }
    return true;
}
// ================================================================================================
// BPlusTree class

BPlusTree::BPlusTree() {
    root = 0;
    head = 0;
    tail = 0;
}

BPlusTree::~BPlusTree() {
    clearTree();
}

void BPlusTree::clearTree() {
    TreeNode* pNode = root;
    if(root != 0) {
        pNode->deleteNode();
        delete pNode;
    }
    head = 0;
    tail = 0;
    root = 0;
}

bool BPlusTree::search(int key) {
    // return the leaf node that might contain this key
    LeafNode* cur = searchLeaf(key);
    if (cur == 0) {
        return false;
    }
    int i = 0;
    // search key in LeafNode
    while((cur->getKey(i) < key) && (i < cur->getCount())) {i++;}
    if (cur->getKey(i) == key) {
        return true;
    }
    return false;
}

/*
 To insert a key into a B+ tree:
 (1) If this key already exist in B+ Tree, return false
 (2) Find the LeafNode where this key is supposed to insert in:
        a. If the LeafNode is not full, insert the key into this LeafNode
        b. If the LeafNode is full and it doesn't have parent node, split the leafNode and creat a rootnode
        c. If the LeafNode is full and its parent node is not full, split the LeafNode and insert the newLeaf into parent node pointer, update the key of parent node
        d. If the LeafNode is full and its parent node is full, split the leafnode and parent node, recursively find a parent node that is not full.
 */

bool BPlusTree::insert(int key) {
    // no duplicate keys in the tree, if key exist in the tree, return false
    if (search(key) == true) {
        return false;
    }
    LeafNode* cur = searchLeaf(key);
    // if cur == 0 then this is an empty tree, creat a new LeafNode and insert the key
    if (cur == 0) {
        cur = new LeafNode();
        head = cur;
        tail = cur;
        root = cur;
    }
    // if LeafNode is not full, insert directly
    if (cur->getCount() < MAX_KEY) {
        return cur->insert(key);
    }
    // if LeafNode is full, split it into two LeafNode, add new LeafNode into double linked list
    LeafNode* newLeaf = new LeafNode();
    int newKey = cur->split(newLeaf);
    // insert the newLeaf node into double linkedList
    LeafNode* tempt = cur->next;
    cur->next = newLeaf;
    newLeaf->prev = cur;
    newLeaf->next = tempt;
    if (tempt == 0) {
        tail = newLeaf;
    } else {
        tempt->prev = newLeaf;
    }
    // check insert the key into which LeafNode
    if (key < newKey) {
        cur->insert(key); // insert into original leaf node
    } else {
        newLeaf->insert(key); // insert into newleaf node
    }
    // if the LeafNode is full and it doesn't have parent node, split the LeafNode and create a new RootNode
    InnerNode* parent = (InnerNode*) cur->getParent();
    if (parent == 0) {
        TreeNode* newRoot = new InnerNode();
        // creat a newRoot node
        newRoot->setPointer(0, cur);
        newRoot->setKey(0, newKey);
        newRoot->setPointer(1, newLeaf);
        newRoot->setCount(1);
        // set parent node
        cur->setParent(newRoot);
        newLeaf->setParent(newRoot);
        // update new root
        root = newRoot;
        return true;
    }
    // If the LeafNode is full and it has a parent, do insertInnerNode()
    return insertInnerNode(parent, newKey, newLeaf);
}

// find the LeafNode where this key is suppose to be in
LeafNode* BPlusTree::searchLeaf(int key){
    TreeNode* cur = root;
    while (cur != 0) {
        if (cur->getType() == LEAF_NODE) {
            break;
        }
        int i = 0;
        // find the first key that is greater than key!!!, so that we can use getPointer(i)
        while ((cur->getKey(i) <= key) && (i < cur->getCount())) {i++;}
        cur = cur->getPointer(i);
        // DEBUG: cout << cur->getKey() << "->";
    }
    return (LeafNode*)cur;
}
// pNode is the InnerNode We want to insert key into key array, rightson is the LeafNode Pointer we want to insert into pNode's pointer array
bool BPlusTree::insertInnerNode(InnerNode* pNode, int key, TreeNode* rightSon) {
    if ((pNode == 0) || (pNode->getType() == LEAF_NODE)) {
        return false;
    }
    // If the InnerNode is not full, insert key and pointer into this node
    if (pNode->getCount() < MAX_KEY) {
        return pNode->insert(key, rightSon);
    }
    InnerNode* pBrother = new InnerNode();
    // If InnerNode is full, Split InnerNdoe
    int newKey = pNode->split(pBrother, key);
    if (pNode->getCount() < pBrother->getCount()) {
        pNode->insert(key, rightSon);
    } else if(pNode->getCount() > pBrother->getCount()) {
        pBrother->insert(key, rightSon);
    } else {
        pBrother->setPointer(0, rightSon);
        rightSon->setParent(pBrother);
    }
    // If the InnerNode is full and it doesn't have a parent pointer, split the InnerNode and creat a new RootNode
    InnerNode* parent = (InnerNode*) pNode->getParent();
    if (parent == 0) {
        parent = new InnerNode();
        parent->setPointer(0, pNode);
        parent->setKey(0, newKey);
        parent->setPointer(1, pBrother);
        pNode->setParent(parent);
        pBrother->setParent(parent);
        parent->setCount(1);
        root = parent;
        return true;
    }
    // If InnerNode is full and its parent is full, split InnerNode and its parent until find a parent that is not full
    return insertInnerNode(parent, newKey, pBrother);
}

// print BPlusTree with layer
void BPlusTree::printTree() {
    deque<TreeNode*> queue;
    queue.push_back(root);
    while (!queue.empty()) {
        int size = (int) queue.size();
        for (int i = 0; i < size; i++) {
        	// expand the node
        	TreeNode* cur = queue.front();
        	queue.pop_front();
            cur->printNode();
        	// generate more
        	if (cur->getType() != LEAF_NODE) {
            	for (int i = 0; i <= cur->getCount(); i++) {
                	queue.push_back(cur->getPointer(i));
            	}
			}
        }
        cout << endl;
    }
}

/*
 1. If the LeafNode's count >= MID_V, just remove the key from LeafNode, update its InnerNode's key
 2. If the LeafNode's count < MID_V, find left or right brother LeafNode:
 	a. If the brother Node's count > MID_V, borrow the closest key to this LeafNode, update brother's key
 	b. If the brother Node's count = MID_V, merge two LeafNode together, merge two InnerNode key too.
 	c. If the Node's count is < MID_V after merge, do it recursively
 */

bool BPlusTree::remove(int key) {
    // if this key doesn't exist in tree, just return false
    if (search(key) == false) {
        return false;
    }
    // else remove it from leaf node
    LeafNode* leaf = searchLeaf(key);
    leaf->remove(key);
    InnerNode* parent = (InnerNode*) leaf->getParent();
    // If the LeafNode don't have parent, which means this LeafNode is RootNode.
    if (parent == 0) {
        // If leaf is empty after removal, then the whole tree is empty
        if (leaf->getCount() == 0) {
            delete leaf;
            root = 0;
            head = 0;
            tail = 0;
        }
        return true;
    }
    // If the LeafNode has parent and the LeafNode's count >= MID_V after removal, update the InnerNode key
    if (leaf->getCount() >= MID_V) {
        for(int i = 0; (parent->getKey(i) <= key) && (i < parent->getCount()); i++) {
            if (parent->getKey(i) == key) {
                parent->setKey(i, leaf->getKey(0));
            }
        }
        return true;
    }
    // If LeafNode's count < MID_V, find its left or right brother
    int flag = LEFT;
    // In this case, both leaf and pBrother are LeafNode, so move one key from brother is easy than InnerNode
    LeafNode* pBrother = (LeafNode*) leaf->getBrother(flag);
    // Case 2a, if brother node's count > MID_V
    int newKey = 0; // the key we want to move from brother node to leaf node
    // If its brother node's count > MID_V, borrow one key from its brother and update brother's key and parent's key
    if (pBrother->getCount() > MID_V) {
        // if the brother is from left, take the last key
        if (flag == LEFT) {
            newKey = pBrother->getKey(pBrother->getCount() - 1);
        // if the brother is from right, take the first key
        } else {
            newKey = pBrother->getKey(0);
        }
    	leaf->insert(newKey);
    	pBrother->remove(newKey);
    	// update the key of brother and leaf
    	// If brother is from left, we only need to update leaf's parent key
    	if (flag == LEFT) {
        	for (int i = 0; i <= parent->getCount(); i++) {
            	// find the pointer of index, the update leaf's parent key
            	if (parent->getPointer(i) == leaf && i > 0) {
                	parent->setKey(i - 1, leaf->getKey(0));
            	}
        	}
    	// If brother is from right, we need to update both leaf and pBrother's parent key
    	} else {
        	for (int i = 0; i <= parent->getCount(); i++) {
            	if (parent->getPointer(i) == pBrother && i > 0) {
                	parent->setKey(i - 1, pBrother->getKey(0));
            	}
            	if (parent->getPointer(i) == leaf && i > 0) {
                	parent->setKey(i - 1, leaf->getKey(0));
            	}
        	}
    	}
    	return true;
    }
    // Case 2b, if brother node's count = MID_V, merge two node together
    int newData = 0;
    if (flag == LEFT) {
        pBrother->merge(leaf);
        // means that the newData we want to delete is the original key
        if (key < leaf->getKey(0)) {
        	newData = key;
        } else {
        	newData = leaf->getKey(0); // the key we want to delete from parent node
        }
        // Delelte leaf in double linkedlist
        LeafNode* temp = leaf->next;
        pBrother->next = temp;
    	if (temp == 0) {
        	tail = pBrother;
    	} else {
        	temp->prev = pBrother;
    	}
    	delete leaf;
    } else {
        leaf->merge(pBrother);
        // update key, in case we remove the frist key before
        for (int i = 0; i <= parent->getCount(); i++) {
            if (parent->getPointer(i) == leaf) {
                parent->setKey(i - 1, leaf->getKey(0));
            }
        }
        newData = pBrother->getKey(0); // the key we want to delete from parent node
        // delete pBrother in double linkedlist
        LeafNode* temp = pBrother->next;
        leaf->next = temp;
        if (temp == 0) {
            tail = leaf;
        } else {
            temp->prev = leaf;
        }
        delete pBrother;
    }
    return removeInnerNode(parent, newData);
}

// Remove the key from InnerNode pNode
bool BPlusTree::removeInnerNode(InnerNode* pNode, int key) {
    bool success = pNode->remove(key);
    if (success == false) {
        return false;
    }
    InnerNode* parent = (InnerNode*) pNode->getParent();
    // If pNode doesn't have parent
    if (parent == 0) {
        // if pNode doesn't have key, set the first pointer as root
        if (pNode->getCount() == 0) {
            root = pNode->getPointer(0);
            pNode->getPointer(0)->setParent(0); //remember to set parent as null
            delete pNode;
        }
        return true;
    }
    // If pNode's count >= MID_V after removal
    if (pNode->getCount() >= MID_V) {
        //
        for (int i = 0; (parent->getKey(i) <= key && i <= parent->getCount()); i++) {
            if (parent->getKey(i) == key) {
                parent->setKey(i, pNode->getKey(0));
            }
        }
        return true;
    }
    int flag = LEFT;
    InnerNode* pBrother = (InnerNode*) pNode->getBrother(flag);
    // if pBrother's count >= MID_V
    if (pBrother->getCount() > MID_V) {
        pNode->moveOneKey(pBrother);
        // If the pBrother is to the left of pNode, update pNode's parent key when move a key from pBrother to pNode
        if (flag == LEFT) {
            for (int i = 0; i <= parent->getCount(); i++) {
                if (parent->getPointer(i) == pNode && i > 0) {
                    parent->setKey(i - 1, pNode->getPointer(0)->getKey(0));
                }
            }
        // If pBrother is to the right of pNode, update pBrother and pNode's parent key when move a key from pBrother to pNode
        } else {
            for (int i = 0; i <= parent->getCount(); i++) {
                if (parent->getPointer(i) == pBrother && i > 0) {
                    parent->setKey(i - 1, pBrother->getKey(0));
                }
                if (parent->getPointer(i) == pNode && i > 0) {
                    parent->setKey(i - 1, pNode->getKey(0));
                }
            }
        }
        return true;
    }
    // merge pNode and pBrother, No matter what merge to the node that have less count, so that parent node don't need to modify pointer
    int newData = 0;
    if (flag == LEFT) {
        pBrother->merge(pNode);
        newData = pNode->getPointer(0)->getKey(0);
        delete pNode;
    } else {
        pNode->merge(pBrother);
        newData = pBrother->getPointer(0)->getKey(0);
        delete pBrother;
    }
    return removeInnerNode(parent, newData);
}
