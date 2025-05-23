#include "HashTable.h"

unsigned long ht_string2int(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

//TODO: Implement
unsigned int ht_hash(char *key, unsigned int slots) {
    // Use the provided ht_string2int function to get a hash value
    unsigned long hash_value = ht_string2int(key);
    // Map the hash value to a slot index using modulo
    return hash_value % slots;
}