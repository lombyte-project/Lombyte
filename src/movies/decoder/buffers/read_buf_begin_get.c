#include "rnc/movies/decoder/read_buf.h"

s32 read_buf_begin_get(struct ReadBuf *buf, u8 **ptr) __asm__("FUN_0023b9d8");

s32 read_buf_begin_get(struct ReadBuf *ring_buffer, u8 **read_start_out) {
    if (ring_buffer->count != 0) {
        *read_start_out = ring_buffer->data + (ring_buffer->put - ring_buffer->count + ring_buffer->size) % ring_buffer->size;
    }
    return ring_buffer->count;
}

extern __typeof__(read_buf_begin_get) func_0023B9D8 __attribute__((alias("FUN_0023b9d8")));
extern __typeof__(read_buf_begin_get) readBufBeginGet__FP7ReadBufPPUc
    __attribute__((alias("FUN_0023b9d8")));
