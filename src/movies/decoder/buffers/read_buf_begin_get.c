#include "rnc/movies/decoder/read_buf.h"

s32 read_buf_begin_get(struct ReadBuf *buf, u8 **ptr) __asm__("FUN_0023b9d8");

s32 read_buf_begin_get(struct ReadBuf *buf, u8 **ptr) {
    if (buf->count != 0) {
        *ptr = buf->data + (buf->put - buf->count + buf->size) % buf->size;
    }
    return buf->count;
}

extern __typeof__(read_buf_begin_get) func_0023B9D8 __attribute__((alias("FUN_0023b9d8")));
extern __typeof__(read_buf_begin_get) readBufBeginGet__FP7ReadBufPPUc
    __attribute__((alias("FUN_0023b9d8")));
