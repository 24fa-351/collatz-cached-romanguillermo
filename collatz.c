#include "collatz.h"
#include "cache.h"

uint64_t collatz_steps(uint64_t n) {
    if (n == 1) {
        return 0;
    }
    // Check if the result is in the cache
    if (cache_has(n)) {
        return cache_value_for(n);
    }
    
    uint64_t next_num;
    if (n % 2 == 0) {
        next_num = n / 2;
    } else {
        next_num = 3 * n + 1;
    }

    if (next_num == 1) return 1;
    
    uint64_t steps = 1 + collatz_steps(next_num);

    cache_insert(n, steps);
    
    return steps;
}
