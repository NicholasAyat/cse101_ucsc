#include "VocabBST.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Create a new token node
TokenNode* createNode(const char* token, int token_id) {
    TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode));
    newNode->token = strdup(token);
    newNode->token_id = token_id;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->height = 0;
    newNode->balanceFactor = 0;
    return newNode;
}

// Insert a token into the BST
TokenNode* insertToken(TokenNode* root, const char* token, int token_id, int* inserted) {
    if (root == NULL) {
        *inserted = 1;
        return createNode(token, token_id);
    }
    int cmp = strcmp(token, root->token);
    if (cmp < 0) {
        root->left = insertToken(root->left, token, token_id, inserted);
    } else if (cmp > 0) {
        root->right = insertToken(root->right, token, token_id, inserted);
    } else {
        *inserted = 0; // Duplicate
    }
    return root;
}

static TokenNode* findMin(TokenNode* node) {
    TokenNode* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Find a token with depth reporting
TokenNode* findToken(TokenNode* root, const char* token, int* token_id_out, int* depth_out) {
    int depth = 0;
    TokenNode* current = root;
    while (current != NULL) {
        int cmp = strcmp(token, current->token);
        if (cmp == 0) {
            *token_id_out = current->token_id;
            *depth_out = depth;
            return current;
        } else if (cmp < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
        depth++;
    }
    return NULL;
}

// Delete a token
TokenNode* deleteToken(TokenNode* root, const char* token, int* deleted) {
    if (root == NULL) {
        *deleted = 0;
        return root;
    }

    int cmp = strcmp(token, root->token);
    if (cmp < 0) {
        root->left = deleteToken(root->left, token, deleted);
    } else if (cmp > 0) {
        root->right = deleteToken(root->right, token, deleted);
    } else {
        *deleted = 1;
        if (root->left == NULL) {
            TokenNode* temp = root->right;
            free(root->token);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            TokenNode* temp = root->left;
            free(root->token);
            free(root);
            return temp;
        }
        TokenNode* temp = findMin(root->right);
        free(root->token);
        root->token = strdup(temp->token);
        root->token_id = temp->token_id;
        int dummy_deleted; // The token is guaranteed to be found
        root->right = deleteToken(root->right, temp->token, &dummy_deleted);
    }
    return root;
}

// Inorder traversal
void inorderPrint(TokenNode* root) {
    if (root != NULL) {
        inorderPrint(root->left);
        printf("%s ", root->token);
        inorderPrint(root->right);
    }
}

// Preorder traversal
void preorderPrint(TokenNode* root) {
    if (root != NULL) {
        printf("%s ", root->token);
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}

// Postorder traversal
void postorderPrint(TokenNode* root) {
    if (root != NULL) {
        postorderPrint(root->left);
        postorderPrint(root->right);
        printf("%s ", root->token);
    }
}

// Free entire tree
void freeTree(TokenNode** root) {
    if (*root == NULL) return;
    freeTree(&((*root)->left));
    freeTree(&((*root)->right));
    free((*root)->token);
    free(*root);
    *root = NULL;
}
