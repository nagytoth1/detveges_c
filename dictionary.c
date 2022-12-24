#include "dictionary.h"

static Dictionary* table[HASHSIZE]; /* pointer table */
unsigned hash(char* key)
{
    unsigned h_value;
    for (h_value = 0; *key != '\0'; key++)
        h_value = *key + 31 * h_value;
    return h_value % HASHSIZE;
}

struct Dictionary* lookup(char* key)
{
    struct Dictionary* foundEntry;
    for (foundEntry = table[hash(key)]; foundEntry != NULL; foundEntry = foundEntry->next)
        if (strcmp(key, foundEntry->key) == 0)
            return foundEntry; /* found */
    return NULL; /* not found */
}

char* duplicate_str(char* key) /* make a duplicate of key */
{
    char* duplicate;
    duplicate = (char*)malloc(strlen(key) + 1); /* +1 for �\0� */
    if (duplicate != NULL)
        strcpy(duplicate, key);
    return duplicate;
}

struct Dictionary* put(char* key, char* value)
{
    struct Dictionary* entry;
    unsigned h_value;
    if ((entry = lookup(key)) == NULL) { /* not found */
        entry = (struct Dictionary*)malloc(sizeof(*entry));
        if (entry == NULL || (entry->key = duplicate_str(key)) == NULL)
            return NULL;
        h_value = hash(key);
        entry->next = table[h_value];
        table[h_value] = entry;
    }
    else /* already there */
        free((void*)entry->value); /*free previous value */
    if ((entry->value = duplicate_str(value)) == NULL)
        return NULL;
    return entry;
}