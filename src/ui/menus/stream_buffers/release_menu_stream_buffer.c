#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/audio/streaming/request_audio_stream_break.h"

s32 release_menu_stream_buffer(s32 buffer_address) __asm__("FUN_00225cd8");

s32 release_menu_stream_buffer(s32 buffer_address) {
    s32  i;
    u8  *base;
    s32 *buffer_flags;

    /* base is a separate local so the +4 (flags) stays its own addiu. */
    base = (u8 *)menu_stream_buffers;
    buffer_flags = (s32 *)(base + 4);
    i = 0;
    while (i < MENU_STREAM_BUFFER_COUNT) {
        if (buffer_flags[-1] == buffer_address) {
            if ((*buffer_flags & MENU_STREAM_BUFFER_ALLOCATED) != 0) {
                if ((*buffer_flags & MENU_STREAM_BUFFER_READ_ACTIVE) != 0) {
                    *buffer_flags ^= MENU_STREAM_BUFFER_READ_ACTIVE;
                    if (menu_system.pending_buffer != 0) {
                        request_audio_stream_break(buffer_address);
                        menu_system.pending_buffer = 0;
                    }
                }
                *buffer_flags &= ~MENU_STREAM_BUFFER_ALLOCATED;
                return 0;
            }
        }
        i++;
        buffer_flags += 2;
    }
    return 0;
}

extern __typeof__(release_menu_stream_buffer) func_00225CD8 __attribute__((alias("FUN_00225cd8")));
