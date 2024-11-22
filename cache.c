#include "cache.h"
#include <stdlib.h>

typedef struct {
    uint64_t key;
    uint64_t value;
    uint64_t timestamp; // For LRU
    uint64_t order;     // For FIFO
} cache_entry;

static cache_entry *cache = NULL;
static uint64_t cache_capacity = 0;
static uint64_t cache_size = 0;
static CachePolicy current_policy;
static uint64_t global_time = 0;
static uint64_t global_order = 0;

// Statistics for cache hits
static uint64_t total_lookups = 0;
static uint64_t cache_hits = 0;

void cache_init(uint64_t size, CachePolicy policy) {
    cache_capacity = size;
    current_policy = policy;
    cache_size = 0;
    if (cache_capacity > 0) {
        cache = (cache_entry *)malloc(sizeof(cache_entry) * cache_capacity);
    }
}

int cache_has(uint64_t key) {
    if (current_policy == POLICY_NONE) {
        return 0;
    }
    total_lookups++;
    for(uint64_t i = 0; i < cache_size; i++) {
        if(cache[i].key == key) {
            cache_hits++;
            if(current_policy == POLICY_LRU) {
                cache[i].timestamp = global_time++;
            }
            return 1;
        }
    }
    return 0;
}

uint64_t cache_value_for(uint64_t key) {
    for(uint64_t i = 0; i < cache_size; i++) {
        if(cache[i].key == key) {
            return cache[i].value;
        }
    }
    return -1; // Not found
}

void cache_insert(uint64_t key, uint64_t value) {
    if (current_policy == POLICY_NONE || cache_capacity == 0) {
        return;
    }

    // Check if key already exists (optional, since cache_has was called before)
    for(uint64_t i = 0; i < cache_size; i++) {
        if(cache[i].key == key) {
            cache[i].value = value;
            if(current_policy == POLICY_LRU) {
                cache[i].timestamp = global_time++;
            }
            return;
        }
    }

    if(cache_size < cache_capacity) {
        // Insert new entry
        cache[cache_size].key = key;
        cache[cache_size].value = value;
        if(current_policy == POLICY_LRU) {
            cache[cache_size].timestamp = global_time++;
        }
        if(current_policy == POLICY_FIFO) {
            cache[cache_size].order = global_order++;
        }
        cache_size++;
    } else {
        // Evict based on policy
        uint64_t evict_index = 0;
        if(current_policy == POLICY_LRU) {
            uint64_t min_time = cache[0].timestamp;
            evict_index = 0;
            for(uint64_t i = 1; i < cache_size; i++) {
                if(cache[i].timestamp < min_time) {
                    min_time = cache[i].timestamp;
                    evict_index = i;
                }
            }
        }
        else if(current_policy == POLICY_FIFO) {
            uint64_t min_order = cache[0].order;
            evict_index = 0;
            for(uint64_t i = 1; i < cache_size; i++) {
                if(cache[i].order < min_order) {
                    min_order = cache[i].order;
                    evict_index = i;
                }
            }
        }
        // Replace evicted entry with new entry
        cache[evict_index].key = key;
        cache[evict_index].value = value;
        if(current_policy == POLICY_LRU) {
            cache[evict_index].timestamp = global_time++;
        }
        if(current_policy == POLICY_FIFO) {
            cache[evict_index].order = global_order++;
        }
    }
}

void cache_free_cache(void) {
    if(cache != NULL) {
        free(cache);
        cache = NULL;
    }
}

double get_cache_hit_percentage(void) {
    if(total_lookups == 0) return 0.0;
    return ((double)cache_hits / (double)total_lookups) * 100.0;
}
