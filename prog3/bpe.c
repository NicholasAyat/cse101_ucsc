#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dictionary.h"

#define MAX_LINE_LEN 1024 // Maximum length of a line read from file or stdin
#define MAX_TOKEN_LEN 64  // Maximum length of a token (word or subword)
#define MAX_TOKENS 256    // Maximum number of tokens per sentence
#define MAX_ITER 10       // Maximum number of BPE merge iterations

// Structure to store a sentence 
// (actually one word, stored as a token sequence, e.g. corpus[0]: "lower" → ['l', 'o', 'w', 'e', 'r', '</w>']) as a sequence of tokens
typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int token_count;
} Sentence;

// Global corpus and dictionary structures
Sentence corpus[1000];
int corpus_size = 0;

// Global dictionaries:
// - token_to_id: maps token (string) → token ID (string)
Dictionary *token_to_id;
int next_token_id = 0;  // Counter to assign unique token IDs

/**
 * @brief Prints a key-value pair in the format "key: value".
 *
 * @param data Pointer to the KVPair to print.
 */
void print_KVPair(void *data) {
    printf("%s: %s\n", ((KVPair *)data)->key, (char*)((KVPair *)data)->value);
}

/**
 * @brief Adds a new token to the token-to-id and id-to-token dictionaries.
 *        Does nothing if the token already exists.
 *
 * @param token The token string to add.
 */
void add_token(char *token) {
    // Check if the token is already present
    if (dictionary_find(token_to_id, token) != NULL)
        return;

    // Convert the current ID counter to string
    char *id_str = malloc(16);
    sprintf(id_str, "%d", next_token_id);

    // Insert token → ID mapping
    char *token_copy = strdup(token);
    KVPair *fwd = malloc(sizeof(KVPair));
    fwd->key = strdup(token_copy);
    fwd->value = strdup(id_str);
    dictionary_insert(token_to_id, fwd);

    next_token_id++;  // Increment the next available ID
}

/**
 * 
 * @brief Finds the most frequent adjacent token pair in the corpus.
 * 
 * Step-by-step:
 * 1. For each sentence in the corpus:
 *    a. For each adjacent token pair:
 *       - Count how often this pair appears in the entire corpus.
 * 2. Track the pair with the highest frequency.
 * 3. Store the best pair's left and right tokens in the provided output buffers.
 * 4. Return the highest count found.
 * 5. Called by bpe_train() to determine which pair to merge.
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param best_left Output: the left token of the most frequent pair.
 * @param best_right Output: the right token of the most frequent pair.
 * @return int The frequency count of the most frequent pair.
 */
int find_best_pair(Sentence corpus[], int corpus_size, char* best_left, char* best_right) {
    // Create a temporary dictionary to count pair frequencies
    Dictionary *pair_counts = dictionary_create(101, NULL);
    int max_count = 0;
    
    // First pass: count all adjacent pairs
    for (int i = 0; i < corpus_size; i++) {
        for (int j = 0; j < corpus[i].token_count - 1; j++) {
            // Create a key for this pair
            char pair_key[MAX_TOKEN_LEN * 2 + 1];
            sprintf(pair_key, "%s %s", corpus[i].tokens[j], corpus[i].tokens[j + 1]);
            
            // Check if this pair is already counted
            KVPair *existing = dictionary_find(pair_counts, pair_key);
            if (existing) {
                // Increment count
                int count = atoi((char*)existing->value) + 1;
                char count_str[16];
                sprintf(count_str, "%d", count);
                existing->value = strdup(count_str);
                
                // Update max if needed
                if (count > max_count) {
                    max_count = count;
                    strcpy(best_left, corpus[i].tokens[j]);
                    strcpy(best_right, corpus[i].tokens[j + 1]);
                }
            } else {
                // Add new pair with count 1
                KVPair *new_pair = malloc(sizeof(KVPair));
                new_pair->key = strdup(pair_key);
                new_pair->value = strdup("1");
                dictionary_insert(pair_counts, new_pair);
                
                // Update max if this is the first pair
                if (max_count == 0) {
                    max_count = 1;
                    strcpy(best_left, corpus[i].tokens[j]);
                    strcpy(best_right, corpus[i].tokens[j + 1]);
                }
            }
        }
    }
    
    // Clean up
    dictionary_destroy(pair_counts);
    return max_count;
}

/**
 * @brief Merges the specified token pair across the entire corpus.
 * 
 * Step-by-step:
 * 1. Create a merged token by concatenating best_left + best_right.
 * 2. For each sentence in the corpus:
 *    a. Scan tokens left-to-right.
 *    b. When a matching pair is found:
 *       - Replace left token with merged token.
 *       - Shift all following tokens left by one.
 *       - Decrement the sentence's token count.
 * 3. Called by bpe_train() after finding the best pair.
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param best_left The left token of the pair.
 * @param best_right The right token of the pair.
 */
void merge_pair(Sentence corpus[], int corpus_size, char* best_left, char* best_right) {
    // Create the merged token
    char merged_token[MAX_TOKEN_LEN];
    sprintf(merged_token, "%s%s", best_left, best_right);
    
    // For each sentence in the corpus
    for (int i = 0; i < corpus_size; i++) {
        // Scan tokens left-to-right
        for (int j = 0; j < corpus[i].token_count - 1; j++) {
            // Check if this is the pair we want to merge
            if (strcmp(corpus[i].tokens[j], best_left) == 0 && 
                strcmp(corpus[i].tokens[j + 1], best_right) == 0) {
                
                // Replace left token with merged token
                strcpy(corpus[i].tokens[j], merged_token);
                
                // Shift all following tokens left by one
                for (int k = j + 1; k < corpus[i].token_count - 1; k++) {
                    strcpy(corpus[i].tokens[k], corpus[i].tokens[k + 1]);
                }
                
                // Decrement token count
                corpus[i].token_count--;
                
                // Since we merged tokens, we need to check the current position again
                j--;
            }
        }
    }
}

/**
 * @brief Runs the full BPE training loop.
 * 
 * Step-by-step:
 * 1. Repeat up to max_iter times:
 *    a. Call find_best_pair() to identify the most frequent pair.
 *    b. If no pair is found, stop early.
 *    c. Call merge_pair() to merge the pair in the corpus.
 * 2. Print progress after each iteration. e.g. printf("Iteration %d: merging '%s' + '%s'\n", iter + 1, best_left, best_right);
 *
 * @param corpus Array of Sentence structs.
 * @param corpus_size Number of sentences in the corpus.
 * @param max_iter Maximum number of merge iterations.
 */
void bpe_train(Sentence corpus[], int corpus_size, int max_iter) {
    for (int iter = 0; iter < max_iter; iter++) {
        char best_left[MAX_TOKEN_LEN];
        char best_right[MAX_TOKEN_LEN];
        
        // Find the most frequent pair
        int max_count = find_best_pair(corpus, corpus_size, best_left, best_right);
        
        // If no pairs found, stop early
        if (max_count == 0) {
            break;
        }
        
        // Print progress
        printf("Iteration %d: merging '%s' + '%s'\n", iter + 1, best_left, best_right);
        
        // Merge the pair
        merge_pair(corpus, corpus_size, best_left, best_right);
    }
}

/**
 * @brief Tokenizes an input sentence (multi-word sentence) using the trained BPE vocabulary.
 * 
 * Step-by-step:
 * 1. For each word in the input:
 *    a. Append </w> to mark end of word.
 *    b. Scan from left to right:
 *       - Greedily find the longest matching token using dictionary_find().
 *       - If no match, treat as unknown character and fallback for unknown single character. e.g. printf("[UNK(%s)] ", fallback);
 *    c. Print the matched token and its ID (or UNK). e.g. printf("Word '%s': ", word); printf("[%s -> %s] ", matched_token, (char*)matched_kv->value);
 *
 * @param input Input sentence string.
 * @param token_to_id Dictionary mapping tokens to IDs.
 */
void greedy_bpe_tokenize(char *input, Dictionary *token_to_id) {
    char *word = strtok(input, " \n");
    while (word) {
        printf("Word '%s': ", word);
        
        // Create a copy of the word with </w> appended
        char word_with_marker[MAX_LINE_LEN];
        sprintf(word_with_marker, "%s</w>", word);
        int len = strlen(word_with_marker);
        
        // Greedy matching from left to right
        int pos = 0;
        while (pos < len) {
            // Try to find the longest matching token
            int best_match_len = 0;
            char best_match[MAX_TOKEN_LEN] = "";
            
            // Try all possible lengths from current position
            for (int match_len = 1; match_len <= len - pos; match_len++) {
                char candidate[MAX_TOKEN_LEN];
                strncpy(candidate, word_with_marker + pos, match_len);
                candidate[match_len] = '\0';
                
                // Check if this token exists in vocabulary
                KVPair *kv = dictionary_find(token_to_id, candidate);
                if (kv) {
                    best_match_len = match_len;
                    strcpy(best_match, candidate);
                }
            }
            
            if (best_match_len > 0) {
                // Found a match
                KVPair *kv = dictionary_find(token_to_id, best_match);
                printf("[%s -> %s] ", best_match, (char*)kv->value);
                pos += best_match_len;
            } else {
                // No match found, treat as unknown character
                char unknown[2] = {word_with_marker[pos], '\0'};
                printf("[UNK(%s)] ", unknown);
                pos++;
            }
        }
        printf("\n");
        
        word = strtok(NULL, " \n");
    }
}

/**
 * @brief Main entry point: reads corpus, trains BPE, builds vocabulary, and processes input.
 *
 * @param argc Argument count.
 * @param argv Argument vector (expects corpus filename as first argument).
 * @return int Exit code.
 */
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <corpus_file>\n", argv[0]);
        return 1;
    }

    // Open corpus file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open file");
        return 1;
    }

    // Initialize dictionaries
    token_to_id = dictionary_create(101, print_KVPair);

    char line[MAX_LINE_LEN];

    // Step 1: Read corpus and split into character-level tokens
    while (fgets(line, sizeof(line), fp)) {
        char *word = strtok(line, " \n");
        while (word) {
            Sentence *s = &corpus[corpus_size++];
            s->token_count = 0;

            // Break word into individual characters
            for (int i = 0; i < strlen(word); i++) {
                char ch[2] = {word[i], '\0'};
                strcpy(s->tokens[s->token_count++], ch);
            }

            // Add end-of-word marker
            strcpy(s->tokens[s->token_count++], "</w>");
            word = strtok(NULL, " \n");
        }
    }
    fclose(fp);

    // Step 2: Run BPE merge training
    bpe_train(corpus, corpus_size, MAX_ITER);

    // Step 3: Build final vocabulary from unique tokens
    for (int i = 0; i < corpus_size; i++) {
        for (int j = 0; j < corpus[i].token_count; j++) {
            add_token(corpus[i].tokens[j]);
        }
    }

    printf("\nVocabulary:\n");
    dictionary_print(token_to_id);

    // Step 4: Process user input for BPE tokenization
    printf("\nEnter sentence to tokenize (or Ctrl+D to exit):\n");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character
        greedy_bpe_tokenize(line, token_to_id);
    }

    // Clean up
    dictionary_destroy(token_to_id);

    return 0;
}
