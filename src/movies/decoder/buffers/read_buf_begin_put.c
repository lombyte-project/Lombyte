#include "rnc/movies/decoder/read_buf.h"

s32 read_buf_begin_put(struct ReadBuf *buf, u8 **ptr) __asm__("func_0023B960");

s32 read_buf_begin_put(struct ReadBuf *buf, u8 **put_destination_out) {
    s32 free = buf->size - buf->count;

    if (free != 0) {
        *put_destination_out = buf->data + buf->put;
    }
    return free;
}

/* Recovered original symbol name. */
extern __typeof__(read_buf_begin_put) readBufBeginPut__FP7ReadBufPPUc
    __attribute__((alias("func_0023B960")));
