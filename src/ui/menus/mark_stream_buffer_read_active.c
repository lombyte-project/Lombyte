#include "types.h"
#include "rnc/rendering/graphics_buffer.h"

s32 mark_stream_buffer_read_active(s32 arg0) __asm__("FUN_00225dd8");

s32 mark_stream_buffer_read_active(s32 buffer_address) {
    s32 count = 0;
    u8 *base = (u8 *)graphics_buffer_descriptors;
    s32 *entry = (s32 *)(base + 4);
loop:
    count += 1;
    if (entry[-1] != buffer_address) {
        entry += 2;
        if (count >= 5) {
            return 1;
        }
        goto loop;
    }
    *entry |= 4;
    return 0;
}
