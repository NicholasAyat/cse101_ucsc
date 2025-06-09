#ifndef VOCAB_AVL_H
#define VOCAB_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure representing a node in the AVL tree (or basic BST)
typedef struct TokenNode {
    char* token;                   // The string token (e.g., "apple")
    int token_id;                 // Unique ID assigned to the token
    struct TokenNode* left;      // Pointer to the left child
    struct TokenNode* right;     // Pointer to the right child

    // Extra fields for AVL bonus, optional to use for prog4_basic
    struct TokenNode* parent;    // Pointer to parent node (optional)
    int height;                  // Height of the node (for AVL balancing)
    int balanceFactor;           // Balance factor: left height - right height
} TokenNode;

/**
 * Create a new node with the given token and token_id.
 * Initializes left/right/parent pointers and AVL-related fields.
 * 
 * @param token      The string token to store in the node.
 * @param token_id   The unique integer ID of the token.
 * @return           Pointer to the newly created node.
 */
TokenNode* createNode(const char* token, int token_id);

/**
 * Insert a token into the BST or AVL tree.
 * If the token already exists, no changes are made.
 * 
 * @param root       Pointer to the root of the tree.
 * @param token      Token to insert.
 * @param token_id   ID to assign if the token is new.
 * @param inserted   Output flag (1 if inserted, 0 if duplicate).
 * @return           Updated root of the tree.
 */
TokenNode* insertToken(TokenNode* root, const char* token, int token_id, int* inserted);

/**
 * Delete a token from the BST or AVL tree.
 * If the token does not exist, the tree remains unchanged.
 * If the node has two children, replaces it with its inorder successor.
 * Frees memory associated with the token string and the node.
 * 
 * @param root       Pointer to the root of the tree.
 * @param token      Token to delete.
 * @param deleted    Output flag (1 if deleted, 0 if not found).
 * @return           Updated root of the tree.
 */
TokenNode* deleteToken(TokenNode* root, const char* token, int* deleted);

/**
 * Search for a token in the tree and report its ID and depth if found.
 * 
 * @param root           Pointer to the root of the tree.
 * @param token          Token to search for.
 * @param token_id_out   Output: ID of the token if found.
 * @param depth_out      Output: Depth of the node (root is depth 0).
 * @return               Pointer to the found node, or NULL if not found.
 */
TokenNode* findToken(TokenNode* root, const char* token, int* token_id_out, int* depth_out);

/**
 * Print the tokens in lexicographic (inorder) order.
 * 
 * @param root       Pointer to the root of the tree.
 */
void inorderPrint(TokenNode* root);

/**
 * Print the tokens in preorder (Root, Left, Right) order.
 * 
 * @param root       Pointer to the root of the tree.
 */
void preorderPrint(TokenNode* root);

/**
 * Print the tokens in postorder (Left, Right, Root) order.
 * 
 * @param root       Pointer to the root of the tree.
 */
void postorderPrint(TokenNode* root);

/**
 * Free all memory associated with the tree.
 * 
 * @param root       Double pointer to the root. Sets *root to NULL after freeing.
 */
void freeTree(TokenNode** root);

#endif 