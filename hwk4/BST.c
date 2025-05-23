#include "BST.h"

// Create a new BST node
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Insert a value into the BST
Node* insertNode(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }
    
    // If value already exists, do nothing
    if (value == root->value) {
        return root;
    }
    
    // Insert in left subtree if value is smaller
    if (value < root->value) {
        root->left = insertNode(root->left, value);
    }
    // Insert in right subtree if value is larger
    else {
        root->right = insertNode(root->right, value);
    }
    
    return root;
}

// Find minimum value node in a subtree
Node* findMin(Node* node) {
    Node* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// Delete a node from BST
Node* deleteNode(Node* root, int value) {
    if (root == NULL) {
        return root;
    }
    
    // Find the node to delete
    if (value < root->value) {
        root->left = deleteNode(root->left, value);
    }
    else if (value > root->value) {
        root->right = deleteNode(root->right, value);
    }
    else {
        // Node with only one child or no child
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        
        // Node with two children: Get the inorder successor (smallest in right subtree)
        Node* temp = findMin(root->right);
        
        // Copy the inorder successor's content to this node
        root->value = temp->value;
        
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->value);
    }
    return root;
}

// Helper function to find depth of a node
int findDepth(Node* root, int value, int depth) {
    if (root == NULL) {
        return -1;
    }
    
    if (root->value == value) {
        return depth;
    }
    
    if (value < root->value) {
        return findDepth(root->left, value, depth + 1);
    }
    
    return findDepth(root->right, value, depth + 1);
}

// Search for a value in the BST and print Found (Depth: d) or Not found
void findNode(Node* root, int value) {
    int depth = findDepth(root, value, 0);
    if (depth != -1) {
        printf("Found (Depth: %d)\n", depth);
    } else {
        printf("Not found\n");
    }
}

// Inorder traversal
void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->value);
        inorderTraversal(root->right);
    }
}

// Preorder traversal
void preorderTraversal(Node* root) {
    if (root != NULL) {
        printf("%d ", root->value);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

// Postorder traversal
void postorderTraversal(Node* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->value);
    }
}

// Free the entire tree
void freeTree(Node** root) {
    if (*root != NULL) {
        freeTree(&((*root)->left));
        freeTree(&((*root)->right));
        free(*root);
        *root = NULL;
    }
}
