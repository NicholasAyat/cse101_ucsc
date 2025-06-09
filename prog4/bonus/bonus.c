#include "VocabAVL.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_CMD_LEN 128
#define MAX_TOK_LEN 64

int main() {
    TokenNode* root = NULL;
    int n;
    int next_token_id = 0;

    // Read the number of initial tokens
    scanf("%d", &n);

    // Consume the rest of the line after the number
    while (getchar() != '\n');

    // Read initial tokens from a single line
    char line[n * (MAX_TOK_LEN + 1)];
    if (fgets(line, sizeof(line), stdin)) {
        char* token = strtok(line, " \n");
        while (token != NULL) {
            int inserted = 0;
            root = insertToken(root, token, next_token_id, &inserted);
            if (inserted) {
                next_token_id++;
            }
            token = strtok(NULL, " \n");
        }
    }


    char command[MAX_CMD_LEN];
    char argument[MAX_TOK_LEN];

    // Command processing loop
    while (scanf("%s", command) == 1) {
        if (strcmp(command, "insert") == 0) {
            scanf("%s", argument);
            int inserted = 0;
            root = insertToken(root, argument, next_token_id, &inserted);
            if (inserted) {
                printf("Inserted: %s (id=%d)\n", argument, next_token_id);
                next_token_id++;
            } else {
                printf("Token already exists\n");
            }
        } else if (strcmp(command, "delete") == 0) {
            scanf("%s", argument);
            int deleted = 0;
            root = deleteToken(root, argument, &deleted);
            if (deleted) {
                printf("Deleted: %s\n", argument);
            }
        } else if (strcmp(command, "find") == 0) {
            scanf("%s", argument);
            int token_id_out;
            int depth_out;
            if (findToken(root, argument, &token_id_out, &depth_out)) {
                printf("Found: %s (id=%d) (Depth: %d)\n", argument, token_id_out, depth_out);
            } else {
                printf("Not found for the token %s\n", argument);
            }
        } else if (strcmp(command, "inorder") == 0) {
            inorderPrint(root);
            printf("\n");
        } else if (strcmp(command, "preorder") == 0) {
            preorderPrint(root);
            printf("\n");
        } else if (strcmp(command, "postorder") == 0) {
            postorderPrint(root);
            printf("\n");
        } else if (strcmp(command, "exit") == 0) {
            break;
        }
    }

    freeTree(&root);

    return 0;
} 