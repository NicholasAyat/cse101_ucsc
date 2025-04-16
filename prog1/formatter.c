#include "List.h"
#include "Stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define MAX_FILENAME_LENGTH 100

// Current position in list for iteration
static int currentPos = 0;

/*
 * Function: printString
 * -------------------
 * Prints a string stored as void* data. Used as the printer function
 * for the Stack ADT to print filenames.
 * 
 * data: void pointer to a string to be printed
 */
void printString(void *data) {
    printf("%s\n", (char*)data);
}

/*
 * Function: createFormattedFilename
 * -------------------------------
 * Creates a new filename for the formatted output file based on the input
 * filename, alignment type, and width.
 * 
 * inputFile: original input filename
 * align: alignment type ('L', 'R', or 'C')
 * width: line width for formatting
 * returns: dynamically allocated string containing the new filename
 */
char* createFormattedFilename(const char* inputFile, char align, int width) {
    char* result = malloc(MAX_FILENAME_LENGTH);
    char* base = strdup(inputFile);
    char* dot = strrchr(base, '.');
    if (dot) *dot = '\0';
    snprintf(result, MAX_FILENAME_LENGTH, "%s.%c%d", base, align, width);
    free(base);
    return result;
}

/*
 * Function: readWordsToList
 * -----------------------
 * Reads words from the input file into a List ADT.
 * Each word is dynamically allocated and stored in the list.
 * Handles multiple spaces, tabs, and preserves punctuation.
 * 
 * input: input file pointer to read from
 * returns: List containing the words
 */
ListPtr readWordsToList(FILE* input) {
    ListPtr wordList = createList(printString);
    char line[MAX_LINE_LENGTH];
    char word[MAX_WORD_LENGTH];
    
    // Reset file position to start
    fseek(input, 0, SEEK_SET);
    
    // Read line by line
    while (fgets(line, MAX_LINE_LENGTH, input)) {
        char* ptr = line;
        int wordStart = 0;
        int i;
        
        // Process each character in the line
        for (i = 0; line[i] != '\0'; i++) {
            if (isspace(line[i])) {
                if (i > wordStart) {
                    // Copy word and add to list
                    strncpy(word, line + wordStart, i - wordStart);
                    word[i - wordStart] = '\0';
                    char* wordCopy = strdup(word);
                    appendList(wordList, wordCopy);
                }
                wordStart = i + 1;
            }
        }
        
        // Handle last word in line if exists
        if (i > wordStart) {
            strncpy(word, line + wordStart, i - wordStart);
            // Remove newline if present
            if (word[i - wordStart - 1] == '\n') {
                word[i - wordStart - 1] = '\0';
            } else {
                word[i - wordStart] = '\0';
            }
            if (strlen(word) > 0) {
                char* wordCopy = strdup(word);
                appendList(wordList, wordCopy);
            }
        }
    }
    
    return wordList;
}

/*
 * Function: formatText
 * ------------------
 * Formats text from a List of words according to specified alignment and width.
 * Uses the List ADT to process words and create formatted lines.
 * 
 * wordList: List containing the words to format
 * output: output file pointer
 * align: alignment type ('L', 'R', or 'C')
 * width: maximum line width
 */
void formatText(ListPtr wordList, FILE* output, char align, int width) {
    char line[MAX_LINE_LENGTH] = "";
    int lineLen = 0;
    int wordCount = 0;
    
    // Reset position counter for list traversal
    currentPos = 0;
    
    // Process each word in the list
    while (currentPos < lengthList(wordList)) {
        char* word = (char*)getList(wordList, currentPos);
        int wordLen = strlen(word);
        
        // If this word would exceed line width, print current line and start new one
        if (lineLen + (wordCount > 0 ? 1 : 0) + wordLen > width) {
            // Handle alignment
            if (align == 'R') {  // Right align
                int padding = width - lineLen;
                for (int i = 0; i < padding; i++) fprintf(output, " ");
            } else if (align == 'C') {  // Center align
                int padding = (width - lineLen) / 2;
                for (int i = 0; i < padding; i++) fprintf(output, " ");
            }
            fprintf(output, "%s\n", line);
            
            // Reset line
            line[0] = '\0';
            lineLen = 0;
            wordCount = 0;
        }
        
        // Add word to line
        if (wordCount > 0) {
            strcat(line, " ");
            lineLen++;
        }
        strcat(line, word);
        lineLen += wordLen;
        wordCount++;
        
        currentPos++;
    }
    
    // Print last line if not empty
    if (lineLen > 0) {
        if (align == 'R') {  // Right align
            int padding = width - lineLen;
            for (int i = 0; i < padding; i++) fprintf(output, " ");
        } else if (align == 'C') {  // Center align
            int padding = (width - lineLen) / 2;
            for (int i = 0; i < padding; i++) fprintf(output, " ");
        }
        fprintf(output, "%s\n", line);
    }
}

/*
 * Function: cleanupWordList
 * -----------------------
 * Frees all dynamically allocated words in the list and destroys the list.
 * 
 * wordList: pointer to the ListPtr to be cleaned up
 */
void cleanupWordList(ListPtr *wordList) {
    if (*wordList == NULL) return;
    
    // Free each word in the list
    for (int i = 0; i < lengthList(*wordList); i++) {
        char* word = (char*)getList(*wordList, i);
        if (word) free(word);
    }
    destroyList(wordList);
}

int main(int argc, char **argv) {
    char command[MAX_WORD_LENGTH];
    char inputFile[MAX_FILENAME_LENGTH] = "";
    FILE* input = NULL;
    StackPtr fileStack = createStack(printString);
    ListPtr wordList = NULL;
    
    while (scanf("%s", command) == 1) {
        if (strcmp(command, "open") == 0) {
            // Clean up previous word list if it exists
            if (wordList != NULL) {
                cleanupWordList(&wordList);
            }
            
            if (scanf("%s", inputFile) != 1) {
                printf("Invalid command\n");
                continue;
            }
            input = fopen(inputFile, "r");
            if (input) {
                printf("Opened file: %s\n", inputFile);
                // Read words into list
                wordList = readWordsToList(input);
                // Print words for verification (optional)
                #ifdef DEBUG
                printf("Words read from file:\n");
                printList(wordList);
                #endif
            } else {
                printf("Failed to open file: %s\n", inputFile);
                inputFile[0] = '\0';
            }
        }
        else if (strcmp(command, "format") == 0) {
            char align;
            int width;
            char alignStr[2];
            
            if (scanf("%s %d", alignStr, &width) != 2 || 
                (alignStr[0] != 'L' && alignStr[0] != 'R' && alignStr[0] != 'C') || 
                width <= 0) {
                printf("Invalid command\n");
                continue;
            }
            
            if (!input || inputFile[0] == '\0' || wordList == NULL) {
                printf("No input file opened\n");
                continue;
            }
            
            align = alignStr[0];
            char* newFilename = createFormattedFilename(inputFile, align, width);
            FILE* output = fopen(newFilename, "w");
            
            if (output) {
                formatText(wordList, output, align, width);
                fclose(output);
                printf("Created new file: %s\n", newFilename);
                
                // Push filename to stack for undo tracking
                char* stackFilename = strdup(newFilename);
                pushStack(fileStack, stackFilename);
            } else {
                printf("Failed to create output file\n");
            }
            
            free(newFilename);
        }
        else if (strcmp(command, "undo") == 0) {
            char* lastFile = (char*)popStack(fileStack);
            if (lastFile) {
                if (remove(lastFile) == 0) {
                    printf("Deleted new file: %s\n", lastFile);
                } else {
                    printf("Failed to delete file: %s\n", lastFile);
                }
                free(lastFile);
            } else {
                printf("Undo failed: format command stack is empty\n");
            }
        }
        else if (strcmp(command, "exit") == 0) {
            break;
        }
        else {
            printf("Invalid command\n");
        }
    }
    
    // Cleanup
    if (input) {
        fclose(input);
    }
    
    // Clean up word list
    if (wordList != NULL) {
        cleanupWordList(&wordList);
    }
    
    // Free any remaining filenames in stack and destroy stack
    while (lengthStack(fileStack) > 0) {
        char* filename = (char*)popStack(fileStack);
        free(filename);
    }
    destroyStack(&fileStack);
    
    return 0;
}
