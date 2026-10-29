#include "rnc/movies/decoder/read_buf.h"

s32 read_buf_end_get(struct ReadBuf *buf, s32 size) __asm__("GetStateTableWord");

s32 read_buf_end_get(struct ReadBuf *buf, s32 size) {
    s32 taken;
    s32 remaining_count;

    taken = buf->count;
    remaining_count = taken;
    if (size < taken) {
        taken = size;
    }
    remaining_count -= taken;
    buf->count = remaining_count;
    return taken;
}
