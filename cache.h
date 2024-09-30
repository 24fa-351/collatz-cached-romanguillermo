#ifndef CACHE_H
#define CACHE_H

#include <stdint.h>

typedef enum {
    POLICY_NONE,
    POLICY_LRU,
    POLICY_FIFO
} CachePolicy;

// Initialize the cache with a given size and policy
void cache_init(uint64_t size, CachePolicy policy);

// Check if a key exists in the cache
int cache_has(uint64_t key);

// Retrieve the value for a given key from the cache
uint64_t cache_value_for(uint64_t key);

// Insert a key-value pair into the cache
void cache_insert(uint64_t key, uint64_t value);

// Free the cache memory
void cache_free_cache(void);

// Get the cache hit percentage
double get_cache_hit_percentage(void);

#endif
