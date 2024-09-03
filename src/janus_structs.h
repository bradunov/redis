#ifndef __JANUS_STRUCT_H
#define __JANUS_STRUCT_H

#include <stdint.h>
#include <stddef.h>

// Janus context
#define MAX_JANUS_KEY 80
typedef struct {
    char key[MAX_JANUS_KEY];               /* Key name. */
    uint64_t eviction_duration;            /* Time from start of eviction to evicting this key */
    uint32_t memory_full;
    size_t memory_total;
    size_t memory_logical;
    size_t memory_tofree;
    uint32_t memory_level;                 /* Memory level 1/10000ths */
    uint32_t keys_freed;
    uint32_t result;                       /* EVICT_OK, EVICT_RUNNING, EVICT_FAIL */ 
} JanusEvictKeyCtx;


typedef struct {
    uint64_t max_time_us;
    uint64_t min_time_us;
    uint32_t max_keys_freed;
    uint32_t min_keys_freed;
    uint32_t max_memory_level;
    uint32_t min_memory_level;
} evict_stats;

#endif