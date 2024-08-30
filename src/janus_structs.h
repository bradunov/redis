#ifndef __JANUS_STRUCT_H
#define __JANUS_STRUCT_H

#include <stdint.h>
#include <stddef.h>

// Janus context
#define MAX_JANUS_KEY 80
typedef struct {
    char key[MAX_JANUS_KEY];               /* Key name. */
    uint64_t eviction_duration;            /* Time from start of eviction to evicting this key */
    int memory_full;
    size_t memory_total;
    size_t memory_logical;
    size_t memory_tofree;
    int memory_level;                      /* Memory level 1/10000ths */
    int keys_freed;
    int result;                            /* EVICT_OK, EVICT_RUNNING, EVICT_FAIL */ 
} JanusEvictKeyCtx;

#endif