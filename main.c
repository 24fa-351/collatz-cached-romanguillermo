#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include "cache.h"
#include "collatz.h"

uint64_t rand_between(uint64_t min, uint64_t max) {
    uint64_t range = max - min + 1;
    return min + (rand() % range);
}

int main(int argc, char *argv[]) {
    uint64_t num_of_values = atoi(argv[1]);
    uint64_t min = atoi(argv[2]);
    uint64_t max = atoi(argv[3]);
    char *cache_policy_str = argv[4];
    uint64_t cache_size = atoi(argv[5]);

    CachePolicy policy;
    if(strcmp(cache_policy_str, "LRU") == 0) {
        policy = POLICY_LRU;
    }
    else if(strcmp(cache_policy_str, "FIFO") == 0) {
        policy = POLICY_FIFO;
    }
    else {
        policy = POLICY_NONE;
    }

    cache_init(cache_size, policy);

    FILE *file = fopen("output.csv", "w");

    srand(time(NULL));

    for(uint64_t i = 0; i < num_of_values; i++) {
        uint64_t rando = rand_between(min, max);
        uint64_t random = rand() % (max - min + 1) + min;
        uint64_t input_num = random;
        uint64_t steps = collatz_steps(random);
        fprintf(file, "%lu, %lu\n", input_num, steps);
    }

    fclose(file);

    double hit_percentage = get_cache_hit_percentage();
    printf("Cache hit percentage: %.2f%%\n", hit_percentage);

    cache_free_cache();

    return 0;
}
