#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"

#define MAX_LINE_LEN 1024    // Maximum length of a line read from file or stdin
#define MAX_TOKEN_LEN 64     // Maximum length of a token (word)
#define INIT_VOCAB_SIZE 256  // Initial vocabulary size (not actively used here)

// Global dictionaries:
// - token_to_id: maps token (string) → token ID (string)
// - id_to_token: maps token ID (string) → token (string)
Dictionary *token_to_id;
Dictionary *id_to_token;
int next_token_id = 0;  // Counter to assign unique token IDs

// Function to print a key-value pair, used by dictionary_print
void print_KVPair(void *data) {
    printf("%s: %s\n", ((KVPair *)data)->key, (char*)((KVPair *)data)->value);
}

// Add a new token (word) to the vocabulary if it doesn’t already exist
void add_token(char *token) {
    // Check if the token is already in the dictionary
    if (dictionary_find(token_to_id, token) != NULL)
        return;  // Token already exists, skip

    // Convert the next_token_id (integer) to a string
    char *id_str = malloc(16);
    sprintf(id_str, "%d", next_token_id);

    // Insert into token_to_id: token → ID
    char *token_copy = strdup(token);
    KVPair *fwd = malloc(sizeof(KVPair));
    fwd->key = strdup(token_copy);
    fwd->value = strdup(id_str);
    dictionary_insert(token_to_id, fwd);

    // Insert into id_to_token: ID → token
    KVPair *rev = malloc(sizeof(KVPair));
    rev->key = strdup(id_str);
    rev->value = token_copy;  // Reuse the copy here
    dictionary_insert(id_to_token, rev);

    next_token_id++;  // Increment the unique ID counter
}

// Given a line of text, print out the token IDs for each known word
void tokenize_line(char *line) {
    char *token = strtok(line, " \n");
    while (token) {
        KVPair *kv = dictionary_find(token_to_id, token);
        if (kv) {
            printf("%s ", (char *)kv->value);  // Print token’s ID
        } else {
            printf("UNK ");  // If unknown, print "UNK"
        }
        token = strtok(NULL, " \n");
    }
    printf("\n");
}

int main(int argc, char **argv) {
    // Check that a corpus file is provided
    if (argc < 2) {
        printf("Usage: %s <corpus_file>\n", argv[0]);
        return 1;
    }

    // Open the corpus file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    // Create two dictionaries with 101 hash slots
    token_to_id = dictionary_create(101, print_KVPair);
    id_to_token = dictionary_create(101, print_KVPair);

    char line[MAX_LINE_LEN];

    // Read the corpus line by line and build the vocabulary
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, " \n");
        while (word) {
            add_token(word);  // Add each word to the vocabulary
            word = strtok(NULL, " \n");
        }
    }

    fclose(fp);  // Close the corpus file

    // Print out the final vocabulary: token → ID
    printf("Vocabulary:\n");
    dictionary_print(token_to_id);

    // Prompt the user for input to tokenize
    printf("\nEnter sentence to tokenize (or Ctrl+D to exit):\n");
    while (fgets(line, sizeof(line), stdin)) {
        tokenize_line(line);  // Tokenize and print ID sequence
    }

    printf("\n---------------------------------\n");

    // --- Testing section for dictionary_insert ---
    char *temp_token = strdup("hhhhhh");
    KVPair *kvp = malloc(sizeof(KVPair));
    kvp->key = strdup(temp_token);
    kvp->value = strdup("666");
    dictionary_insert(token_to_id, kvp);
    printf("Test dictionary_insert:\n");
    dictionary_print(token_to_id);

    printf("\n---------------------------------\n");

    // --- Testing section for dictionary_delete ---
    dictionary_delete(token_to_id, "hhhhhh");
    printf("Test dictionary_delete:\n");
    dictionary_print(token_to_id);

    // Clean up all allocated memory
    dictionary_destroy(token_to_id);
    dictionary_destroy(id_to_token);

    return 0;
}
