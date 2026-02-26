#include <stdio.h>
#include "../include/hashtable.h"

int main() {
    HashTable* ht = ht_create(8);
    if (!ht) {
        printf("Failed to create hashtable\n");
        return 1;
    }

    ht_put(ht, "apple", 10);
    ht_put(ht, "banana", 20);
    ht_put(ht, "orange", 30);

    ht_put(ht, "banana", 99);

    int v;
    if (ht_get(ht, "banana", &v)) printf("banana=%d\n", v);
    if (ht_get(ht, "apple", &v))  printf("apple=%d\n", v);

    printf("size=%zu capacity=%zu\n", ht_size(ht), ht_capacity(ht));

    ht_remove(ht, "orange");
    printf("after remove orange: size=%zu\n", ht_size(ht));

    if (!ht_get(ht, "orange", &v)) printf("orange not found\n");

    ht_destroy(ht);
    return 0;
}