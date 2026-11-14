#include "rnc/movies/decoder/read_buf.h"

s32 read_buf_end_get(struct ReadBuf *buf, s32 size) __asm__("GetStateTableWord");

s32 read_buf_end_get(struct ReadBuf *read_source, s32 requested_count) {
    s32 taken;
    s32 remaining_count;

    taken = read_source->count;
    remaining_count = taken;
    if (requested_count < taken) {
        taken = requested_count;
    }
    remaining_count -= taken;
    read_source->count = remaining_count;
    return taken;
}
