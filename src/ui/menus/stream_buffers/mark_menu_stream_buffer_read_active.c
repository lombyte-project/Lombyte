#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"

s32 mark_menu_stream_buffer_read_active(s32 buffer_address) __asm__("FUN_00225dd8");

s32 mark_menu_stream_buffer_read_active(s32 buffer_address) {
    s32  count = 0;
    u8  *base = (u8 *)menu_stream_buffers;
    s32 *entry = (s32 *)(base + 4);
loop:
    count += 1;
    if (entry[-1] != buffer_address) {
        entry += 2;
        if (count >= MENU_STREAM_BUFFER_COUNT) {
            return 1;
        }
        goto loop;
    }
    *entry |= MENU_STREAM_BUFFER_READ_ACTIVE;
    return 0;
}
