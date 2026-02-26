#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <stdbool.h>

typedef struct HashTable HashTable;

HashTable* ht_create(size_t initial_capacity);
void ht_destroy(HashTable* ht);

bool ht_put(HashTable* ht, const char* key, int value);   // insert or update
bool ht_get(const HashTable* ht, const char* key, int* out_value);
bool ht_remove(HashTable* ht, const char* key);

size_t ht_size(const HashTable* ht);
size_t ht_capacity(const HashTable* ht);

#endif