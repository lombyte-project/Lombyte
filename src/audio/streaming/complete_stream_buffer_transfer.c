#include "types.h"
#include "rnc/rendering/graphics_buffer.h"
#include "rnc/ui/menus/menu_system.h"

extern void request_audio_stream_break(s32) __asm__("FUN_002166e8");

s32 complete_stream_buffer_transfer(s32 stream_id) __asm__("FUN_00225cd8");

s32 complete_stream_buffer_transfer(s32 stream_id) {
    s32 i;
    u8 *base;
    s32 *p;

    /* base is a separate local so the +4 (flags) stays its own addiu. */
    base = (u8 *)graphics_buffer_descriptors;
    p = (s32 *)(base + 4);
    i = 0;
    while (i < 5) {
        if (p[-1] == stream_id) {
            if ((*p & 2) != 0) {
                if ((*p & 4) != 0) {
                    *p ^= 4;
                    if (menu_system.pending_buffer != 0) {
                        request_audio_stream_break(stream_id);
                        menu_system.pending_buffer = 0;
                    }
                }
                *p &= -3;
                return 0;
            }
        }
        i++;
        p += 2;
    }
    return 0;
}

extern __typeof__(complete_stream_buffer_transfer) func_00225CD8
    __attribute__((alias("FUN_00225cd8")));
