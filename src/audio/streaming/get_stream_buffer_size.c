#include "types.h"
struct StreamBufferEntry {
    s32 unk0;
    s32 unk4;
};

extern u8 D_001D60B8[];
s32 get_stream_buffer_size(s32 stream_id) __asm__("FUN_00225d88");

s32 get_stream_buffer_size(s32 stream_id) {
    struct StreamBufferEntry *entry;
    s32 index;

    entry = D_001D60B8;
    index = 0;
    do {
        index += 1;
        if (entry->unk0 == stream_id) {
            return (entry->unk4 & 1) ? 0x4F000 : 0x11800;
        }
        entry += 1;
    } while (index < 5);
    return -1;
}

extern __typeof__(get_stream_buffer_size) func_00225D88 __attribute__((alias("FUN_00225d88")));
