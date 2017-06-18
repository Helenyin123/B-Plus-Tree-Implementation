//
//  main.cpp
//  BPlusTree
//
//  Created by Chang Yin on 4/25/17.
//  Copyright © 2017 Chang Yin. All rights reserved.
//

#include "BPlusTree.hpp"
// randomly build a tree
void test1(BPlusTree* tree, int count) {
    tree->clearTree();
    for (int i = 0; i < count; i++) {
        int x = rand()%999 + 1;
        tree->insert(x);
    }
    printf("successed!\n");
}

// search a key in the tree
void test2(BPlusTree* tree, int data) {
    bool success = tree->search(data);
    if (success == true) {
        printf("successed!\n");
    } else {
        printf("failed!\n");
    }
}

// insert a data into tree
void test3(BPlusTree* tree, int data) {
    bool success = tree->insert(data);
    if (success == true) {
        printf("successed!\n");
    } else {
        printf("failed!\n");
    }
}
//// remove a data from the tree
void test4(BPlusTree* tree, int data) {
    bool success = tree->remove(data);
    if (success == true) {
        printf("successed!\n");
    } else {
        printf("failed!\n");
    }
}

// print the tree
void test5(BPlusTree* tree) {
    tree->printTree();
}

void testcase(BPlusTree* tree) {
    tree->clearTree();
    cout << "==================================== Correctness Test ========================================" <<endl;
    cout << "insert 2: " << tree->insert(2) << endl;
    tree->printTree();
    cout << "insert 6: " << tree->insert(6) << endl;
    tree->printTree();
    cout << "insert 15: " << tree->insert(15) << endl;
    tree->printTree();
    cout << "insert 25: " << tree->insert(25) << endl;
    tree->printTree();
    cout << "insert 7: " << tree->insert(7) << endl;
    tree->printTree();
    cout << "insert 11: " << tree->insert(11) << endl;
    tree->printTree();
    cout << "insert 8: " << tree->insert(8) << endl;
    tree->printTree();
    cout << "insert 20: " << tree->insert(20) << endl;
    tree->printTree();
    cout << "insert 35: " << tree->insert(35) << endl;
    tree->printTree();
    cout << "insert 16: " << tree->insert(16) << endl;
    tree->printTree();
    cout << "insert 24: " << tree->insert(24) << endl;
    tree->printTree();
    cout << "insert 22: " << tree->insert(22) << endl;
    tree->printTree();
    cout << "insert 23: " << tree->insert(23) << endl;
    tree->printTree();
    cout << "insert 21: " << tree->insert(21) << endl;
    tree->printTree();
    cout << "insert 12: " << tree->insert(12) << endl;
    tree->printTree();
    cout << "insert 14: " << tree->insert(14) << endl;
    tree->printTree();
    //testing moveOneKey() method
    cout << "remove 24: " << tree->remove(24) << endl;
    tree->printTree();
    cout << "remove 16: " << tree->remove(16) << endl;
    tree->printTree();
    cout << "remove 35: " << tree->remove(35) << endl;
    tree->printTree();
    cout << "remove 21: " << tree->remove(21) << endl;
    tree->printTree();
    cout << "remove 25: " << tree->remove(25) << endl;
    tree->printTree();
    cout << "remove 20: " << tree->remove(20) << endl;
    tree->printTree();
    cout << "remove 15: " << tree->remove(15) << endl;
    tree->printTree();
    cout << "remove 11: " << tree->remove(11) << endl;
    tree->printTree();
    cout << "remove 12: " << tree->remove(12) << endl;
    tree->printTree();
    cout << "remove 7: " << tree->remove(7) << endl;
    tree->printTree();
    cout << "remove 8: " << tree->remove(8) << endl;
    tree->printTree();
    cout << "remove 6: " << tree->remove(6) << endl;
    tree->printTree();
    cout << "remove 22: " << tree->remove(22) << endl;
    tree->printTree();
    cout << "remove 14: " << tree->remove(14) << endl;
    tree->printTree();
    cout << "remove 23: " << tree->remove(23) << endl;
    tree->printTree();
    cout << "=====================================================================================" <<endl;
//    // 3 layer to 2 layer
//    cout << "remove 22: " << tree->remove(22) << endl;
//    cout << "remove 25: " << tree->remove(25) << endl;
//    cout << "remove 8: " << tree->remove(8) << endl;
//    cout << "remove 2: " << tree->remove(2) << endl;
//    cout << "remove 24: " << tree->remove(24) << endl;
//    cout << "remove 11: " << tree->remove(11) << endl;
//    cout << "remove 7: " << tree->remove(7) << endl;
//    cout << "remove 35: " << tree->remove(35) << endl;
//    cout << "remove 20: " << tree->remove(20) << endl;
//    cout << "remove 23: " << tree->remove(23) << endl;
//    cout << "remove 6: " << tree->remove(6) << endl;
//    tree->printTree();
}
int main() {
    BPlusTree *tree = new BPlusTree();
    testcase(tree);
    int x = 1;
    int y = 0;
    while (x != 0)
    {
        printf("\n\n");
        printf("    ************************  BPlusTree Data Structure  **********************\n");
        printf("    *              1. Randomly build a BPlusTree                             *\n");
        printf("    *              2. Search a data in the BPlusTree                         *\n");
        printf("    *              3. Insert a data in the BPlusTree                         *\n");
        printf("    *              4. Delete a data in the BPlusTree                         *\n");
        printf("    *              5. Print the BPlusTree                                    *\n");
        printf("    *              0. Exit the program                                       *\n");
        printf("    **************************************************************************\n");
        printf("\n    You choose: ");
        
        
        scanf("%d", &x);
        switch (x)
        {
            case 1:
                printf("Please input total key number:");
                scanf("%d", &y);
                test1(tree, y);
                break;
                
            case 2:
                printf("Please input key you want to search:");
                scanf("%d", &y);
                test2(tree, y);
                break;
                
            case 3:
                printf("Please input data you want to insert:");
                scanf("%d", &y);
                test3(tree, y);
                test5(tree);
                break;
                
            case 4:
                printf("Please input the data you want to remove from the tree:");
                scanf("%d", &y);
                test4(tree, y);
                break;
                
            case 5:
                test5(tree);
                break;

            case 0:
                return 0;
                break;
                
            default:
                break;
        }
    }
    return 0;
}
