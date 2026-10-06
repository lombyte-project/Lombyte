#include "types.h"
struct ReadBuf {
    u8 pad_0[0x50004];
    s32 count;
};

s32 read_buf_end_get(struct ReadBuf *table, s32 requested) {
    s32 taken;
    s32 result;

    taken = table->count;
    result = taken;
    if (requested < taken) {
        taken = requested;
    }
    result -= taken;
    table->count = result;
    return taken;
}
