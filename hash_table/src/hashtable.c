#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/hashtable.h"

typedef struct Entry {
    char* key;
    int value;
    struct Entry* next;
} Entry;

struct HashTable {
    Entry** buckets;
    size_t capacity;
    size_t size;
};

static char* str_dup(const char* s) {
    size_t n = strlen(s);
    char* copy = (char*)malloc(n + 1);
    if (!copy) return NULL;
    memcpy(copy, s, n + 1);
    return copy;
}

// djb2 hash
static unsigned long hash_str(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + (unsigned long)c;
    }
    return hash;
}

static size_t index_for(const char* key, size_t capacity) {
    return (size_t)(hash_str(key) % capacity);
}

static Entry* entry_find(Entry* head, const char* key) {
    while (head) {
        if (strcmp(head->key, key) == 0) return head;
        head = head->next;
    }
    return NULL;
}

static void free_chain(Entry* head) {
    while (head) {
        Entry* next = head->next;
        free(head->key);
        free(head);
        head = next;
    }
}

static bool ht_resize(HashTable* ht, size_t new_capacity) {
    Entry** new_buckets = (Entry**)calloc(new_capacity, sizeof(Entry*));
    if (!new_buckets) return false;

    for (size_t i = 0; i < ht->capacity; i++) {
        Entry* node = ht->buckets[i];
        while (node) {
            Entry* next = node->next;
            size_t idx = index_for(node->key, new_capacity);
            node->next = new_buckets[idx];
            new_buckets[idx] = node;
            node = next;
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
    return true;
}

HashTable* ht_create(size_t initial_capacity) {
    if (initial_capacity < 8) initial_capacity = 8;

    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;

    ht->buckets = (Entry**)calloc(initial_capacity, sizeof(Entry*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }

    ht->capacity = initial_capacity;
    ht->size = 0;
    return ht;
}

void ht_destroy(HashTable* ht) {
    if (!ht) return;
    for (size_t i = 0; i < ht->capacity; i++) {
        free_chain(ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
}

bool ht_put(HashTable* ht, const char* key, int value) {
    if (!ht || !key) return false;

    double load = (ht->capacity == 0) ? 1.0 : (double)ht->size / (double)ht->capacity;
    if (load > 0.75) {
        if (!ht_resize(ht, ht->capacity * 2)) return false;
    }

    size_t idx = index_for(key, ht->capacity);
    Entry* existing = entry_find(ht->buckets[idx], key);

    if (existing) {
        existing->value = value;
        return true;
    }

    Entry* e = (Entry*)malloc(sizeof(Entry));
    if (!e) return false;

    e->key = str_dup(key);
    if (!e->key) {
        free(e);
        return false;
    }

    e->value = value;
    e->next = ht->buckets[idx];
    ht->buckets[idx] = e;
    ht->size++;
    return true;
}

bool ht_get(const HashTable* ht, const char* key, int* out_value) {
    if (!ht || !key || !out_value) return false;

    size_t idx = index_for(key, ht->capacity);
    Entry* e = entry_find(ht->buckets[idx], key);
    if (!e) return false;

    *out_value = e->value;
    return true;
}

bool ht_remove(HashTable* ht, const char* key) {
    if (!ht || !key) return false;

    size_t idx = index_for(key, ht->capacity);
    Entry* curr = ht->buckets[idx];
    Entry* prev = NULL;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev) prev->next = curr->next;
            else ht->buckets[idx] = curr->next;

            free(curr->key);
            free(curr);
            ht->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }

    return false;
}

size_t ht_size(const HashTable* ht) {
    return ht ? ht->size : 0;
}

size_t ht_capacity(const HashTable* ht) {
    return ht ? ht->capacity : 0;
}