// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Counts the size of dictionary entered
int sizeDict = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int hashVal = hash(word); // Converts word to all lower and calls hash function
    node *cursor = table[hashVal]; // Set cursor to node at the hashValue

    if (cursor == NULL)
    {
        return false;
    }
    while (cursor->next != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    if (strcasecmp(cursor->word, word) == 0)
    {
        return true;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    
    int sum = 0;
    for (int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    int hashVal = (sum / strlen(word));
    return hashVal % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");

    // Check if dictionary can be opened
    if (dict == NULL)
    {
        return false;
    }
    // Read dictionary one word at a time
    char buffer[LENGTH];
    while ((fscanf(dict, "%s", buffer)) != EOF)
    {

        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, buffer); // Copy word from buffer to node
        int hashVal = hash(n->word);
        if (table[hashVal] != NULL)
        {
            n->next = table[hashVal];
            table[hashVal] = n;
            sizeDict++;
        }
        else
        {
            n->next = NULL;
            table[hashVal] = n;
            sizeDict++;
        }

    }
    fclose(dict);
    return true;
    
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (sizeDict >= 1)
    {
        return sizeDict++;
    }
    else
    {
        return 0;
    }
    
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        node *cursor = table[i];
        if (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
