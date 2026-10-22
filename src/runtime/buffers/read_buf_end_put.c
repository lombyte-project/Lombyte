/* Commits N bytes after a put, clamped to the free space. */
#include "types.h"

void read_buf_end_put(s32 *buf, s32 n) __asm__("FUN_0023b990");

void read_buf_end_put(s32 *read_ring_buffer, s32 requested_byte_count) {
    s32 *buffer_state = (s32 *)((u8 *)read_ring_buffer + 0x50000);
    s32 space = buffer_state[2] - buffer_state[1];
    s32 committed_byte_count = (requested_byte_count < space) ? requested_byte_count : space;

    buffer_state[0] = (buffer_state[0] + committed_byte_count) % buffer_state[2];
    buffer_state[1] += committed_byte_count;
}

extern __typeof__(read_buf_end_put) func_0023B990 __attribute__((alias("FUN_0023b990")));
/* Recovered original symbol name. */
extern __typeof__(read_buf_end_put) readBufEndPut__FP7ReadBufi
    __attribute__((alias("FUN_0023b990")));
