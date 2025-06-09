#include "VocabAVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper to get height of a node, returns -1 for NULL nodes
static int height(TokenNode* node) {
    return node ? node->height : -1;
}

// Helper to get max of two integers
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Update height and balance factor of a node
static void updateNodeMetrics(TokenNode* node) {
    if (!node) return;
    node->height = 1 + max(height(node->left), height(node->right));
    node->balanceFactor = height(node->left) - height(node->right);
}

// Right rotation
static TokenNode* rightRotate(TokenNode* y) {
    TokenNode* x = y->left;
    TokenNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    updateNodeMetrics(y);
    updateNodeMetrics(x);
    return x;
}

// Left rotation
static TokenNode* leftRotate(TokenNode* x) {
    TokenNode* y = x->right;
    TokenNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    updateNodeMetrics(x);
    updateNodeMetrics(y);
    return y;
}

// Balance the node
static TokenNode* balance(TokenNode* node) {
    updateNodeMetrics(node);
    // Left heavy
    if (node->balanceFactor > 1) {
        // Left-Right case
        if (height(node->left->left) < height(node->left->right)) {
            node->left = leftRotate(node->left);
        }
        return rightRotate(node);
    }
    // Right heavy
    if (node->balanceFactor < -1) {
        // Right-Left case
        if (height(node->right->right) < height(node->right->left)) {
            node->right = rightRotate(node->right);
        }
        return leftRotate(node);
    }
    return node;
}

TokenNode* createNode(const char* token, int token_id) {
    TokenNode* newNode = (TokenNode*)malloc(sizeof(TokenNode));
    if (!newNode) return NULL;
    newNode->token = strdup(token);
    if (!newNode->token) {
        free(newNode);
        return NULL;
    }
    newNode->token_id = token_id;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    newNode->height = 0;
    newNode->balanceFactor = 0;
    return newNode;
}

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
        *inserted = 0;
        return root;
    }

    return balance(root);
}

static TokenNode* findMin(TokenNode* node) {
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

TokenNode* deleteToken(TokenNode* root, const char* token, int* deleted) {
    if (root == NULL) {
        *deleted = 0;
        return NULL;
    }

    int cmp = strcmp(token, root->token);
    if (cmp < 0) {
        root->left = deleteToken(root->left, token, deleted);
    } else if (cmp > 0) {
        root->right = deleteToken(root->right, token, deleted);
    } else {
        *deleted = 1;
        if (!root->left || !root->right) {
            TokenNode* temp = root->left ? root->left : root->right;
            free(root->token);
            free(root);
            return temp;
        } else {
            TokenNode* temp = findMin(root->right);
            free(root->token);
            root->token = strdup(temp->token);
            root->token_id = temp->token_id;
            int dummy;
            root->right = deleteToken(root->right, temp->token, &dummy);
        }
    }
    
    if (root == NULL) return root;

    return balance(root);
}

TokenNode* findToken(TokenNode* root, const char* token, int* token_id_out, int* depth_out) {
    int depth = 0;
    TokenNode* current = root;
    while (current != NULL) {
        int cmp = strcmp(token, current->token);
        if (cmp == 0) {
            *token_id_out = current->token_id;
            *depth_out = depth;
            return current;
        }
        current = (cmp < 0) ? current->left : current->right;
        depth++;
    }
    return NULL;
}

void inorderPrint(TokenNode* root) {
    if (root) {
        inorderPrint(root->left);
        printf("%s ", root->token);
        inorderPrint(root->right);
    }
}

void preorderPrint(TokenNode* root) {
    if (root) {
        printf("%s ", root->token);
        preorderPrint(root->left);
        preorderPrint(root->right);
    }
}

void postorderPrint(TokenNode* root) {
    if (root) {
        postorderPrint(root->left);
        postorderPrint(root->right);
        printf("%s ", root->token);
    }
}

void freeTree(TokenNode** root) {
    if (*root) {
        freeTree(&((*root)->left));
        freeTree(&((*root)->right));
        free((*root)->token);
        free(*root);
        *root = NULL;
    }
} 