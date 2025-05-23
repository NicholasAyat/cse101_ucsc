#include "BST.h"
#include <string.h>

int main() {
    Node* root = NULL;
    int n, value;
    char command[10];
    
    // Read number of initial elements
    scanf("%d", &n);
    
    // Build initial BST
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insertNode(root, value);
    }
    
    // Process commands
    while (1) {
        scanf("%s", command);
        
        if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "insert") == 0) {
            scanf("%d", &value);
            root = insertNode(root, value);
        }
        else if (strcmp(command, "delete") == 0) {
            scanf("%d", &value);
            root = deleteNode(root, value);
        }
        else if (strcmp(command, "find") == 0) {
            scanf("%d", &value);
            findNode(root, value);
        }
        else if (strcmp(command, "inorder") == 0) {
            inorderTraversal(root);
            printf("\n");
        }
        else if (strcmp(command, "preorder") == 0) {
            preorderTraversal(root);
            printf("\n");
        }
        else if (strcmp(command, "postorder") == 0) {
            postorderTraversal(root);
            printf("\n");
        }
    }
    
    // Free the tree before exiting
    freeTree(&root);
    return 0;
}
