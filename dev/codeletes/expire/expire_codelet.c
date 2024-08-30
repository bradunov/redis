// Copyright (c) Microsoft Corporation. All rights reserved.

#include <string.h>

#include "janus_defs.h"
#include "janus_helper.h"


/* Copied from Redis/expire.c */
typedef struct {
    void *db;
    long long now;
    unsigned long sampled; /* num keys checked */
    unsigned long expired; /* num keys expired */
    long long ttl_sum; /* sum of ttl for key with ttl not yet expired */
    int ttl_samples; /* num keys with ttl not yet expired */
} expireScanData;


/* Extended with an extra field */
typedef struct {
    void *db;
    long long now;
    unsigned long sampled; /* num keys checked */
    unsigned long expired; /* num keys expired */
    long long ttl_sum; /* sum of ttl for key with ttl not yet expired */
    int ttl_samples; /* num keys with ttl not yet expired */
    int done;
} expireScanDataResult;



// Output map of type JANUS_MAP_TYPE_RINGBUF.
// The map is used to send out data of type struct Packet.
// It holds a ringbuffer with a total of 3 elements.
janus_ringbuf_map(outmap, expireScanDataResult, 1000);



SEC("janus_generic")
uint64_t
janus_main(void* state)
{

    struct janus_generic_ctx* ctx;
    expireScanData *p, *p_end;
    expireScanDataResult echoResult = {0};
    expireScanData *echo = (expireScanData *)&echoResult;

    ctx = state;

    p = (expireScanData*)ctx->data;
    p_end = (expireScanData*)ctx->data_end;

    if (p + 1 > p_end)
        return 1;

    *echo = *p;
    echoResult.done = ctx->ctx_id;

    // Copy the data that was passed to the codelet to the outmap ringbuffer
    // and send them out.
    if (janus_ringbuf_output(&outmap, &echoResult, sizeof(echoResult)) < 0) {
        return 1;
    }

    return 0;
}
