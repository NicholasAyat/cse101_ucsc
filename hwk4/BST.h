// BST.h - Header File
/**
 * @file BST.h
 * Header file containing the structure of the Binary Search Tree and 
 * the prototypes of the functions to be implemented.
 */

 #ifndef BST_H
 #define BST_H
 
 #include <stdio.h>
 #include <stdlib.h>
 
 // Structure representing a node in the binary search tree
 typedef struct Node {
     int value;
     struct Node *left;
     struct Node *right;
 } Node;
 
 // Create a new BST node with given value
 Node* createNode(int value);
 
 // Insert a value into the BST (no duplicates)
 Node* insertNode(Node* root, int value);
 
 // Delete a node with given value from the BST
 // When deleting a node with two children, replace it with its inorder successor.
 Node* deleteNode(Node* root, int value);
 
 // Inorder traversal (Left, Root, Right)
 void inorderTraversal(Node* root);
 
 // Preorder traversal (Root, Left, Right)
 void preorderTraversal(Node* root);
 
 // Postorder traversal (Left, Right, Root)
 void postorderTraversal(Node* root);

 // Search for a value in the BST and print Found (Depth: d) or Not found
 void findNode(Node* root, int value);
 
 // Free all memory used by the BST
 void freeTree(Node** root);
 
 #endif
 