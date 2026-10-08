#include "types.h"
#include "rnc/ui/menus/menu_system.h"

struct WordCell {
    s32 unk0;
};

extern u8 D_001D60B8[];
extern void request_audio_stream_break(s32) __asm__("FUN_002166e8");

s32 complete_stream_buffer_transfer(s32 stream_id) __asm__("FUN_00225cd8");

s32 complete_stream_buffer_transfer(s32 stream_id) {
    s32 i;
    u8 *base;
    struct WordCell *p;

    base = D_001D60B8;
    p = (struct WordCell *)(base + 4);
    i = 0;
    while (i < 5) {
        if (*(s32 *)((u8 *)p - 4) == stream_id) {
            if ((p->unk0 & 2) != 0) {
                if ((p->unk0 & 4) != 0) {
                    p->unk0 ^= 4;
                    if (menu_system.pending_buffer != 0) {
                        request_audio_stream_break(stream_id);
                        menu_system.pending_buffer = 0;
                    }
                }
                p->unk0 &= -3;
                return 0;
            }
        }
        i++;
        p = (struct WordCell *)((u8 *)p + 8);
    }
    return 0;
}

extern __typeof__(complete_stream_buffer_transfer) func_00225CD8
    __attribute__((alias("FUN_00225cd8")));
