/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

unsigned int wordLoaded = 0;

typedef struct node
{
    bool is_word;
    struct node* children[27];
} node;

node* root;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word) {
    node* current = root;
    int i = 0;

    while (word[i] != '\0') {
        char input = word[i];
        if (word[i] == '\'') {
            input = 'z' + 1;
        }
        int newnum = tolower(input) - 'a';

        if (current->children[newnum] != NULL) {
            current = current->children[newnum];
            i++;
        }
        else {
            return false;
        }
    }
    if (current->is_word == true) {
        return true;
    }
    return false;
}
/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary) {
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL) {
        return false;
    }

    root = malloc(sizeof (node));
    int c = 0; //char from dict
    node* current = NULL;

    while (fgetc(dict) != EOF) {
        //go back 1 byte of the cursor
        fseek(dict, -1, SEEK_CUR);

        // Set the pointer to the root
        current = root;

        for (c = fgetc(dict); c != '\n'; c = fgetc(dict)) {
            // Check for apostrophes
            if (c == '\'') {
                // Set it to one past the highest letter, z
                c = 'z' + 1;
            }

            // Check if the character exists in the trie
            if (current->children[c - 'a'] == NULL) {
                // If the character does not exist, malloc a new node
                current->children[c - 'a'] = malloc(sizeof (node));

                // go to the new node
                current = current->children[c - 'a'];
            } else {
                // go to the node that is already present
                current = current->children[c - 'a'];
            }
        }
        current->is_word = true;
        wordLoaded++;
    }
    fclose(dict);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordLoaded;
}
/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool freeNode(node* thisNode) {
    for (int j = 0; j < 27; j++) {
        if (thisNode->children[j] != NULL) {
            freeNode(thisNode->children[j]);
        }
    }
    free(thisNode);
    return true;
}

bool unload(void) {
    return freeNode(root);
}
