#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"

s32 mark_menu_stream_buffer_read_active(s32 buffer_address) __asm__("FUN_00225dd8");

s32 mark_menu_stream_buffer_read_active(s32 buffer_address) {
    s32  count = 0;
    u8  *stream_buffer_descriptors = (u8 *)menu_stream_buffers;
    s32 *stream_buffer_flags = (s32 *)(stream_buffer_descriptors + 4);
loop:
    count += 1;
    if (stream_buffer_flags[-1] != buffer_address) {
        stream_buffer_flags += 2;
        if (count >= MENU_STREAM_BUFFER_COUNT) {
            return 1;
        }
        goto loop;
    }
    *stream_buffer_flags |= MENU_STREAM_BUFFER_READ_ACTIVE;
    return 0;
}
