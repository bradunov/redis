// Copyright (c) Microsoft Corporation. All rights reserved.

#include <string.h>

#include "janus_defs.h"
#include "janus_helper.h"
#include "../../../src/janus_structs.h"




// Output map of type JANUS_MAP_TYPE_RINGBUF.
janus_ringbuf_map(outmap, JanusEvictKeyCtx, 1000);



SEC("janus_generic")
uint64_t
janus_main(void* state)
{

    struct janus_generic_ctx* ctx;
    JanusEvictKeyCtx *p, *p_end;
    JanusEvictKeyCtx echo = {0};

    ctx = state;

    p = (JanusEvictKeyCtx*)ctx->data;
    p_end = (JanusEvictKeyCtx*)ctx->data_end;

    if (p + 1 > p_end)
        return 1;

    echo = *p;
    
    // Copy the data that was passed to the codelet to the outmap ringbuffer
    // and send them out.
    if (janus_ringbuf_output(&outmap, &echo, sizeof(echo)) < 0) {
        return 1;
    }

    return 0;
}
