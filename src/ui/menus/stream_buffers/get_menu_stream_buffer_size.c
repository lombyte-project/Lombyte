#include "types.h"
#include "rnc/ui/menus/stream_buffers.h"

s32 get_menu_stream_buffer_size(s32 buffer_address) __asm__("FUN_00225d88");

s32 get_menu_stream_buffer_size(s32 buffer_address) {
    struct MenuStreamBuffer *buffer_descriptor;
    s32                      index;

    buffer_descriptor = menu_stream_buffers;
    index = 0;
    do {
        index += 1;
        if (buffer_descriptor->address == buffer_address) {
            return (buffer_descriptor->flags & MENU_STREAM_BUFFER_LARGE) ? 0x4F000 : 0x11800;
        }
        buffer_descriptor += 1;
    } while (index < MENU_STREAM_BUFFER_COUNT);
    return -1;
}

extern __typeof__(get_menu_stream_buffer_size) func_00225D88 __attribute__((alias("FUN_00225d88")));
