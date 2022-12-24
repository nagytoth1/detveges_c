#ifndef DICTIONARY_H
#define DICTIONARY_H
#include <stdlib.h>
#include <string.h>
typedef struct Dictionary { /* table entry: */
    struct Dictionary* next; /* next entry in chain */
    char* key; /* defined key */
    char* value; /* value identified by the key */
} Dictionary;

#define HASHSIZE 101

/* hash: form hash value for string key */
unsigned hash(char* key);

/* lookup: look for key in hashtable */
struct Dictionary* lookup(char* key);

char* duplicate_str(char* key);
/* install: put (key, value) in hashtab */
struct Dictionary* put(char* key, char* value);

#endif // !DICTIONARY_H