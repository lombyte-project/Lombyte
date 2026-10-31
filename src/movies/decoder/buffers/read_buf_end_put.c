#include "rnc/movies/decoder/read_buf.h"

void read_buf_end_put(struct ReadBuf *buf, s32 size) __asm__("FUN_0023b990");

/* Commits size bytes after a put, clamped to the free space. */
void read_buf_end_put(struct ReadBuf *read_ring_buffer, s32 size) {
    s32 free = read_ring_buffer->size - read_ring_buffer->count;
    s32 committed = (size < free) ? size : free;

    read_ring_buffer->put = (read_ring_buffer->put + committed) % read_ring_buffer->size;
    read_ring_buffer->count += committed;
}

extern __typeof__(read_buf_end_put) func_0023B990 __attribute__((alias("FUN_0023b990")));
/* Recovered original symbol name. */
extern __typeof__(read_buf_end_put) readBufEndPut__FP7ReadBufi
    __attribute__((alias("FUN_0023b990")));
