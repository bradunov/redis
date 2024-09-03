// Copyright (c) Microsoft Corporation. All rights reserved.

#include <string.h>

#include "janus_defs.h"
#include "janus_helper.h"
#include "../../../src/janus_structs.h"



// Output map of type JANUS_MAP_TYPE_RINGBUF.
janus_ringbuf_map(outmap, evict_stats, 1000);



struct janus_load_map_def SEC("maps") not_empty = {
    .type = JANUS_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(uint32_t),
    .max_entries = 1,
};

struct janus_load_map_def SEC("maps") last_time = {
    .type = JANUS_MAP_TYPE_ARRAY,
    .key_size = sizeof(int),
    .value_size = sizeof(uint64_t),
    .max_entries = 1,
};

// We store stats in this (single entry) map across runs
struct janus_load_map_def SEC("maps") stats_map = {
  .type = JANUS_MAP_TYPE_ARRAY,
  .key_size = sizeof(int),
  .value_size = sizeof(evict_stats),
  .max_entries = 1,
};





#define DEBUG_PRINT
SEC("janus_generic")
uint64_t
janus_main(void* state)
{

    struct janus_generic_ctx* ctx;
    JanusEvictKeyCtx *p, *p_end;
    uint32_t zero_index = 0;
    uint64_t timestamp;

    uint32_t *not_empty_hist = janus_map_lookup_elem(&not_empty, &zero_index);
    if (!not_empty_hist)
        return 0;

    // Get stats map buffer to save output across invocations
    evict_stats *out = (evict_stats *)janus_map_lookup_elem(&stats_map, &zero_index);
    if (!out)
        return 1;

    timestamp = janus_time_get_ns();

    ctx = state;

    p = (JanusEvictKeyCtx*)ctx->data;
    p_end = (JanusEvictKeyCtx*)ctx->data_end;

    if (p + 1 > p_end)
        return 1;


    // Calculate stats
    if (p->eviction_duration > out->max_time_us)
        out->max_time_us = p->eviction_duration;
    if (p->eviction_duration < out->min_time_us)
        out->min_time_us = p->eviction_duration;
    if (p->keys_freed > out->max_keys_freed)
        out->max_keys_freed = p->keys_freed;
    if (p->keys_freed < out->min_keys_freed)
        out->min_keys_freed = p->keys_freed;
    if (p->memory_level > out->max_memory_level)
        out->max_memory_level = p->memory_level;
    if (p->memory_level < out->min_memory_level)
        out->min_memory_level = p->memory_level;

    *not_empty_hist = 1;


    uint64_t timestamp32 = (uint64_t)(timestamp >> 30);

    uint64_t *last_timestamp = janus_map_lookup_elem(&last_time, &zero_index);
    if (!last_timestamp)
        return 0;

    if (*last_timestamp < timestamp32 && *not_empty_hist)
    {

#ifdef DEBUG_PRINT
        janus_printf_debug("evict_key_stats: %ld %d %d\n", out->max_time_us, out->max_keys_freed, out->max_memory_level);
#endif

        // Copy the data that was passed to the codelet to the outmap ringbuffer
        // and send them out.
        if (janus_ringbuf_output(&outmap, out, sizeof(evict_stats)) < 0) {
            return 1;
        }

        // Reset the info
        out->max_time_us = 0;
        out->min_time_us = UINT64_MAX;
        out->max_keys_freed = 0;
        out->min_keys_freed = UINT32_MAX;
        out->max_memory_level = 0;
        out->min_memory_level = UINT32_MAX;

        *last_timestamp = timestamp32;
        *not_empty_hist = 0;
    }

    return 0;
}
